#include "lua_engine.h"
#include "rooms.h"
#include "gameobjects.h"
#include "sprites.h"

Game* Engine_game = NULL;


int lua_setStep(lua_State *L) {

}

/**
 *  Create a game object,
 * @param Go - The id (adress, pointer) of the game object to set sprite
 * @param w - the width of the spritesheet(size of the file you put in)
 * @param h - the height of the spritesheet(size of the file you put in)
 * @param col - the number of column of the spritesheet
 * @param row - the number of rows of the spritesheet
 * @param animation_speed - the speed in frame per second of the animation
 *
 * @return nil Its ok; nil + string error
 */
int lua_setsprite(lua_State *L) {

    // Récupère la full userdata et vérifie qu'elle a la bonne metatable
    GameObject **ud = luaL_checkudata(L, 1, "GameObjectMeta");
    if (!ud || !*ud) {
        return luaL_error(L, "Invalid GameObject");
    }
    GameObject *go = *ud;


    const char *path = lua_tostring(L, 2);
    if (!path) {
        return luaL_error(L, "Invalid texture path");
    }

    int sprite_w  = luaL_checkinteger(L, 3);
    int sprite_h  = luaL_checkinteger(L, 4);
    int col    = luaL_checkinteger(L, 5);
    int row    = luaL_checkinteger(L, 6);
    int anim_speed = luaL_checkinteger(L, 7);

    if(load_texture(&go->sprite, Engine_game->renderer, path) != 0) {
        return luaL_error(L, "Failed to load texture");
    }

    go->sprite.animSpeed = anim_speed;
    go->sprite.width = sprite_w;
    go->sprite.totalFrames = col * row;
    go->sprite.height = sprite_h;

    if (row == 0) row = 1;
    if (col == 0) col = 1;

    go->sprite.shape.h = go->sprite.height / row;
    go->sprite.shape.w =  go->sprite.width / col;

    return 0;
}

/**
 *  Create a game object,
 * @param roomId - The id (adress, pointer) of the room to add game object
 * @param x - the x position of the object
 * @param y - the y position of the object
 *
 * @return The id (adress, pointer) of thegame object ok; nil + string error
 */
int lua_create_gameobject(lua_State* L) {
    SDL_Log("try to create game object from lua script");
    Room *room = lua_touserdata(L, 1);
    const int x = lua_tointeger(L, 2);
    const int y = lua_tointeger(L, 3);

    if(!x || !y || room == NULL) return luaL_error(L, "Argument missing");

    GameObject *ngo = malloc(sizeof(GameObject));
    if (!ngo) return luaL_error(L, "Failed to allocate GO");

    if (init_gameobject(ngo)) return luaL_error(L, "Failed to init gameobject");
    ngo->free_obj = free_gameobject;

    add_gameobject_in_room(room, ngo);

    ngo->x = x;
    ngo->y = y;

    GameObject **ud = lua_newuserdata(L, sizeof(GameObject*));
    *ud = ngo;
    luaL_getmetatable(L, "GameObjectMeta");
    lua_setmetatable(L, -2);

    return 1;
}

/**
 *  Create a room,
 * @return Pointer of the room (adress), ok; nil + string error
 */
int lua_create_room(lua_State* L) {
    Room* nr = malloc(sizeof(Room));
    if (!nr) return luaL_error(L, "Failed to allocate Room");
    init_room(nr);
    if (add_room(Engine_game, nr) != 0) {
        free(nr);
        lua_pushnil(L);
        lua_pushstring(L, "Error when adding room to game");
        return 2;
    }
    switch_room(Engine_game, 0);
    lua_pushlightuserdata(L, nr);
    return 1;
}

int lua_engine_log(lua_State* L) {

    const char* msg = lua_tostring(L, 1);
    if (!msg) msg = "(null)";
    SDL_Log("[Lua] %s\n", msg);
    // fflush(stdout);
    return 0;
}

int lua_setGameObjectStep(lua_State *L) {
    // 1. Récupère l’userdata (GameObject**)
    GameObject **ud = luaL_checkudata(L, 1, "GameObjectMeta");
    GameObject *go = *ud;

    if (!lua_isfunction(L, 2)) {
        return luaL_error(L, "Expected function");
    }

    // 2. Supprime une éventuelle ancienne fonction
    if (go->lua_step_ref != LUA_NOREF) {
        luaL_unref(L, LUA_REGISTRYINDEX, go->lua_step_ref);
    }

    // 3. Copie la fonction pour la stocker
    lua_pushvalue(L, 2);

    // 4. La "référence" dans le registry
    go->lua_step_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    return 0;
}

int lua_index_gameobject(lua_State *L) {
    GameObject **ud = luaL_checkudata(L, 1, "GameObjectMeta");
    GameObject *go = *ud;
    const char *key = luaL_checkstring(L, 2);

    if (strcmp(key, "x") == 0) {
        lua_pushnumber(L, go->x);
        return 1;
    }
    if (strcmp(key, "y") == 0) {
        lua_pushnumber(L, go->y);
        return 1;
    }

    luaL_getmetatable(L, "GameObjectMeta");
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);
    return 1;
}

int lua_newindex_gameobject(lua_State *L) {
    GameObject **ud = luaL_checkudata(L, 1, "GameObjectMeta");
    GameObject *go = *ud;
    const char *key = luaL_checkstring(L, 2);

    if (strcmp(key, "x") == 0) {
        go->x = luaL_checknumber(L, 3);
        return 0;
    }
    if (strcmp(key, "y") == 0) {
        go->y = luaL_checknumber(L, 3);
        return 0;
    }

    return luaL_error(L, "Unknown property '%s'", key);
}


static const luaL_Reg Engine_funcs[] = {
    {"create_room", lua_create_room},
    {"log", lua_engine_log},

    {"create_gameobject", lua_create_gameobject},
    // {"spawn_object", lua_spawn_object},
    // Ajoute toutes tes fonctions ici ↓↓↓
    {NULL, NULL} // Obligatoire pour marquer la fin
};

static const luaL_Reg GameObject_methods[] = {
    {"setSprite", lua_setsprite},
    {"setStep", lua_setGameObjectStep},

    {NULL, NULL}
};

void register_engine_api(lua_State* L)
{


    lua_newtable(L);

    luaL_setfuncs(L, Engine_funcs, 0);

    lua_setglobal(L, "Engine");

    luaL_newmetatable(L, "GameObjectMeta");
    luaL_setfuncs(L, GameObject_methods, 0);

    lua_pushcfunction(L, lua_index_gameobject);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_newindex_gameobject);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);

}
