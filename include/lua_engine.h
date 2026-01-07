#ifndef LUA_ENGINE_H
#define LUA_ENGINE_H

// Détection automatique du chemin Lua
#ifdef _WIN32
    // Windows
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
#else
    // Linux/Unix
    #include <lua5.4/lua.h>
    #include <lua5.4/lauxlib.h>
    #include <lua5.4/lualib.h>
#endif

// Inclure les headers de ton moteur
#include "game.h"        // Pour Game
#include "rooms.h"       // Pour Room et create_room

// Déclaration du pointeur global
extern Game* Engine_game;

// Fonction pour enregistrer l'API Lua

void register_engine_api(lua_State* L);

#endif