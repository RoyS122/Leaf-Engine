#ifndef LUA_ENGINE_H
#define LUA_ENGINE_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Inclure les headers de ton moteur
#include "game.h"        // Pour Game
#include "rooms.h"       // Pour Room et create_room

// Déclaration du pointeur global
extern Game* Engine_game;

// Fonction pour enregistrer l'API Lua

void register_engine_api(lua_State* L);

#endif