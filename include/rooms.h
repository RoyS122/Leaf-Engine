#ifndef ROOMS_H
#define ROOMS_H

#include "types.h"
// #include <SDL2>
// #include <SDL2/SDL_ttf.h>

int init_room(Room *r);
int add_gameobject_in_room(Room *r, GameObject *go);
int remove_gameobject_in_room(Room *r, int id);
int remove_gameobject_killed_in_room(Room *r);
int free_room(Room *r);





#endif // ROOMS_H