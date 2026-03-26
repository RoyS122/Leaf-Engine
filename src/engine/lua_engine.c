#include "types.h"
#include "lua_engine.h"
#include "lauxlib.h"
#include "lua.h"
#include "rooms.h"
#include "gameobjects.h"
#include "sprites.h"
#include "game.h"        // Pour Game
#include <stdio.h>

Game* Engine_game = NULL;

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


int lua_setGameObjectStep(lua_State *L) {
    // 1. Récupère l’userdata (GameObject**)
    GameObject **ud = luaL_checkudata(L, 1, "GameObjectMeta");
    GameObject *go = *ud;

    if (!lua_isfunction(L, 2)) {
        return luaL_error(L, "Expected function");
    }

    if (go->lua_step_ref != LUA_NOREF) {
        luaL_unref(L, LUA_REGISTRYINDEX, go->lua_step_ref);
    }

    lua_pushvalue(L, 2);


    go->lua_step_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    return 0;
}

/**
 *  Set sprite of the game object,
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

    if (row == 0) row = 1;
    if (col == 0) col = 1;

    go->sprite.animSpeed = anim_speed;
    go->sprite.width = sprite_w / col;
    go->sprite.totalFrames = col * row;
    go->sprite.height = sprite_h / row;

    SDL_Log("Check this totalframe: %d", go->sprite.totalFrames);


    go->sprite.shape.h = go->sprite.height ;
    go->sprite.shape.w =  go->sprite.width ;

    return 0;
}

int lua_create_camera(lua_State *L) {

    float camPosX = luaL_checknumber(L, 1);
    float camPosY = luaL_checknumber(L, 2);
    int camW = luaL_checkinteger(L, 3);
    int camH = luaL_checkinteger(L, 4);

    GameObject **ud_go = (GameObject **)luaL_testudata(L, 5, "GameObjectMeta");
    const GameObject *target = (ud_go != NULL) ? *ud_go : NULL;

    if(camW <= 0 || camH <= 0) {
        luaL_error(L, "Error, camera can't get a negative or null size");
        return 1;
    }


    Camera *cam = malloc(sizeof(Camera));
    if (!cam) return luaL_error(L, "Memory allocation failed");
    cam->width = camW;
    cam->height = camH;
    cam->x = camPosX;
    cam->y = camPosY;
    cam->Follow = target;

    // Retour vers Lua
    Camera **ud_cam = lua_newuserdata(L, sizeof(Camera*));
    *ud_cam = cam;

    luaL_getmetatable(L, "CameraMeta");
    lua_setmetatable(L, -2);


    return 1;
}

int lua_camera_gc(lua_State *L) {
    Camera **ud = luaL_checkudata(L, 1, "CameraMeta");
    if (*ud) {
        printf("DEBUG: Nettoyage mémoire de la caméra\n");
        free(*ud);
    }
    return 0;
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


/**
 *  Custom include for lua script in the engine.
 *
 */
int lua_custom_include(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    const char *scripts_path = lua_tostring(L, lua_upvalueindex(1)); // Get Value inherited from the constructor  of the function
    char path[strlen(scripts_path) + strlen(filename) + 1];

    snprintf(path, sizeof(path), "%s%s", scripts_path, filename);

    if (luaL_dofile(L, path) != LUA_OK) {
        return luaL_error(L, "Error expected file exposing lua function.");
    }


    return 1;
}


/**
 *  Load a script with custom include in closure with the base path in upvalue
 *  ( to allow local script of a game object to reach the other scripts in same directory)
 */
int lua_load_script(lua_State *L) {
    const char *script_path = luaL_checkstring(L, 1);

    int lastSep = -1;
    for(int i = 0; i < strlen(script_path); i ++) {
        if((char)*(script_path + i) == '/' ) {
            lastSep = i;
        }
    }
    int baseSize = 0;
    if(lastSep != -1) {
        baseSize = lastSep + 1;
    }

    char basePath[baseSize + 1];
    int i = 0;
    for(i = 0; i < baseSize; i ++) {
        basePath[i] = (char)*(script_path + i);
    }
    basePath[i] = '\0';
    printf("\n %s \n", basePath);
    printf("\n %s \n", script_path);

    if (luaL_loadfile(L, script_path) != LUA_OK) {
        return luaL_error(L, "Error expected file exposing lua function. (%s)", script_path);
    }

    lua_pushstring(L, basePath);
    lua_pushcclosure(L, lua_custom_include, 1);

    lua_call(L, 1, 1);

    return 1;
}


static const luaL_Reg Engine_funcs[] = {
    {"create_camera", lua_create_camera},
    {"create_room", lua_create_room},
    {"log", lua_engine_log},
    {"load_script", lua_load_script},
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

static const luaL_Reg Camera_methods[] = {
    // {"setSprite", lua_setsprite},
    // {"setStep", lua_setGameObjectStep},

    {NULL, NULL}
};

void register_gameobject(lua_State *L) { // Init methods and indexs of gameobjects

    luaL_newmetatable(L, "GameObjectMeta");
    luaL_setfuncs(L, GameObject_methods, 0);

    lua_pushcfunction(L, lua_index_gameobject);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, lua_newindex_gameobject);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}

void register_camera(lua_State *L) { // Init methods and indexs of cameras
    luaL_newmetatable(L, "CameraMeta");
    luaL_setfuncs(L, Camera_methods, 0);

    lua_pushcfunction(L, lua_camera_gc);
    lua_setfield(L, -2, "__gc");

    lua_pop(L, 1);
}

void register_engine_api(lua_State *L)
{


    lua_newtable(L);

    luaL_setfuncs(L, Engine_funcs, 0);

    lua_setglobal(L, "Engine");

    register_gameobject(L);
    register_camera(L);


}
