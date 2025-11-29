#ifndef BUTTONS_H
#define BUTTONS_H

#include "types.h"

int draw_button(Button *b, SDL_Renderer *renderer);
int init_button(Button *b);
int free_button(Button *b);

#endif // BUTTONS_H