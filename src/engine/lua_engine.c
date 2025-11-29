#include "lua_engine.h"
#include "rooms.h"
// Les includes de ton moteur sont déjà dans lua_engine.h

// Définition du pointeur global
Game* Engine_game = NULL;

int lua_create_room(lua_State* L) {
    Room* nr = malloc(sizeof(Room));
    if (!nr) return luaL_error(L, "Failed to allocate Room");
    init_room(nr);
    
    if (add_room(Engine_game, nr) != 0) {
        free(nr); // libère la mémoire en cas d'erreur
        lua_pushnil(L);                    
        lua_pushstring(L, "Error when adding room to game");
        return 2;                           
    }
    
    lua_pushboolean(L, 1); // succès
    return 1;
}

int lua_engine_log(lua_State* L) {
    
    const char* msg = lua_tostring(L, 1);
        if (!msg) msg = "(null)";
      printf("[Lua] %s\n", msg);
      fflush(stdout);            // important sur Windows
    return 0;
}

// Fonction pour enregistrer toute l'API
void register_engine_api(lua_State* L)
{
    // Créer la table Engine
    lua_newtable(L);
    
    // Ajouter Engine.create_room()
    lua_pushcfunction(L, lua_create_room);
    lua_setfield(L, -2, "create_room");
    lua_pushcfunction(L, lua_engine_log);
    lua_setfield(L, -2, "log");
    
    // Définir la table globale Engine
    lua_setglobal(L, "Engine");
}