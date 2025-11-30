#include "lua_engine.h"
#include "rooms.h"
#include "gameobjects.h"
#include "sprites.h"

Game* Engine_game = NULL;


int lua_setsprite(lua_State *L) {
    GameObject *go = lua_touserdata(L, 1);
    if (!go) {
        return luaL_error(L, "Invalid GameObject");
    }
    
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
    go->sprite.width = sprite_w / col;
    go->sprite.height = sprite_h / row;

    go->sprite.shape.h = go->sprite.height;
    go->sprite.shape.w =  go->sprite.width;
    
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

    lua_pushlightuserdata(L, ngo); 
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
    printf("[Lua] %s\n", msg);
    fflush(stdout);            
    return 0;
}

static const luaL_Reg Engine_funcs[] = {
    {"create_room", lua_create_room},
    {"log", lua_engine_log},
    {"setSprite", lua_setsprite},
    {"create_gameobject", lua_create_gameobject},
    // {"spawn_object", lua_spawn_object},
    // Ajoute toutes tes fonctions ici ↓↓↓
    {NULL, NULL} // Obligatoire pour marquer la fin
};


void register_engine_api(lua_State* L)
{
    lua_newtable(L);                     

    luaL_setfuncs(L, Engine_funcs, 0);  

    lua_setglobal(L, "Engine");         

}