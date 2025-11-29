#ifndef MENUS_H
#define MENUS_H

#include "types.h"

int draw_menu(Menu *m, SDL_Renderer *renderer);
void init_menu(Menu *m);
int add_button(Menu *m, Button *b);
int free_menu(Menu *m);
int remove_button(Menu *m, int id);

#endif // MENUS_H