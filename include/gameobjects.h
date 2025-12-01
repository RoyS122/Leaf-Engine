#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "types.h"

int init_gameobject(GameObject *go);
int free_gameobject(GameObject *go);
int draw_gameobject(GameObject *go, SDL_Renderer *renderer);
int step_gameobject(GameObject *go, Game *game);
int is_collide(const GameObject *a, const GameObject *b);
Vector2D dir_to(const int start_x, const int start_y, const int target_x, const int target_y);

#endif // GAMEOBJECTS_H