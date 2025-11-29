#include "lua_engine.h"
#include "rooms.h"
#include "gameobjects.h"

Game* Engine_game = NULL;



int lua_create_gameobject(lua_State* L) {

    const int room_id = lua_tointeger(L, 1);
    if (Engine_game->RoomNBR <= room_id || room_id < 0) return lual_error(L, "Invalid room ID");
    
    const int go_id = Engine_game->RoomNBR;

    GameObject *ngo = malloc(sizeof(GameObject));
    if (!ngo) return luaL_error(L, "Failed to allocate GO");

    if (init_gameobject(ngo)) return lual_error(L, "Failed to init gameobject");
    ngo->free_obj = free_gameobject;

    lua_pushinteger(L, go_id); // succès
    return 1;
}


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
    
    lua_pushboolean(L, 1); // succès
    return 1;
}

int lua_engine_log(lua_State* L) {
    
    const char* msg = lua_tostring(L, 1);
    if (!msg) msg = "(null)";
    printf("[Lua] %s\n", msg);
    fflush(stdout);            
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