#include <stdio.h>
#include "types.h"


/**
 *  Add a gameobject to room
 * @param r A pointer to the room
 * @param go The gameobject to add to the game
 * @return 0 ok; 1 Invalid parameters; 2 Fail to allocate memory
 */
int add_gameobject_in_room(Room *r, GameObject *go)
{
    if (!r)
    {
        SDL_Log("Invalid parameters: (game: %p)", r);
        return 1;
    }

    r->GameObjectArray = realloc(r->GameObjectArray, (r->GameObjectNBR + 1) * sizeof(*r->GameObjectArray));
    if (r->GameObjectArray == NULL)
    {
        SDL_Log("Failed to allocate memory for new game object");
        return 2;
    }

    r->GameObjectArray[r->GameObjectNBR] = go;
    r->GameObjectNBR++;
    return 0;
};

/**
 *  Remove a gameobject by id
 * @param r A pointer to the room
 * @param id The id of the object to delete
 * @return 0 ok; 1 Invalid parameters; 2 Fail to reallocate memory
 */
int remove_gameobject_in_room(Room *r, int id)
{
	SDL_Log("try to remove ID: %i", id);
    if (id < 0 || id >= r->GameObjectNBR || !r)
    {
        SDL_Log("Invalid parameters: (game: %p id: %i)", r, id);
        return 1;
    }
    r->GameObjectArray[id]->free_obj(r->GameObjectArray[id]);
    for (int i = id; i < r->GameObjectNBR - 1; i++)
    {
        r->GameObjectArray[i] = r->GameObjectArray[i + 1];
    }
	SDL_Log("after free: %i", id);

    r->GameObjectArray = realloc(r->GameObjectArray, (r->GameObjectNBR - 1) * sizeof(*r->GameObjectArray));
    if (r->GameObjectArray == NULL && r->GameObjectNBR - 1 > 0)
    {
        SDL_Log("Failed to reallocate memory after removing game object");
        return 2;
    }
	SDL_Log("after free1: %i", id);
    r->GameObjectNBR--;
	SDL_Log("after free2: %i", id);
    return 0;
};
/**
*  Remove the gameobjects who is killed in the room
* @param r room to clear
* @return 0 ok; 1 Invalid parameters; 2 Fail to remove game object
*/
int remove_gameobject_killed_in_room(Room *r) 
{
    if (!r) {
        SDL_Log("Invalid room pointer");
        return 1;
    }
    
    for (int i = r->GameObjectNBR - 1; i >= 0; i--) {
        if (r->GameObjectArray[i]->killed) {
      
            if (remove_gameobject_in_room(r, i) != 0) return 2;
            
        }
    }
    return 0;
}

/**
 *  Free the attribute and free space allocated to a room (this function only free all his game object from the memory)
 * @param r A pointer to the game to free
 * @return 0 ok; 1 Invalid parameters; 2 Fail to remove game object
 */
int free_room(Room *r) {
	if (!r) {
		SDL_Log("Invalid parameters(room: %p)", r);
		return 1;
	}

	while(r->GameObjectNBR > 0){
		if (remove_gameobject_in_room(r, r->GameObjectNBR - 1)) return 2;
	}
    r->GameObjectArray = NULL;
    

	return 0;
}

/**
 * Initialize the attribute of a room
 * @param r A pointer to the game to free
 * @return 0 ok; 1 Invalid parameters; 
 */
int init_room(Room *r){
    if (!r) {
        SDL_Log("Invalid parameters(room %p)", r);
        return 1;
    }
    r->CurrentGame = NULL;
    r->GameObjectArray = NULL;
    r->GameObjectNBR = 0;
    r->setup = NULL;
    r->free_room = NULL;
    r->step = NULL;
    r->draw = NULL;
    return 0;
}
