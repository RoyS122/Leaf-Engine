#ifndef SPRITES_H
#define SPRITES_H

#include <SDL2/SDL.h>
#include "types.h"

int draw_sprite(SDL_Renderer *renderer, Sprite *sprite);
int update_sprite(Sprite *sprite, int frameRate);
int init_sprite(Sprite *sprite);
int free_sprite(Sprite *s);

#endif // SPRITES_H