#ifndef GAME_H
#define GAME_H

#include "types.h"
#include <SDL2/SDL_ttf.h>



int add_gameobject(Game *game, GameObject *go);
int remove_gameobject(Game *game, int id);

int free_game(Game *game);


int add_menu(Game *game, Menu menu);
int remove_menu(Game *game, int id);

int init_game(Game *game, int framerate, int window_width, int window_height);
void createTextTextureAndRender(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);
int calculate_fps(Uint32 *lastTime, int *frameCount, int *displayedFPS);

int gameloop(Game *game);

int add_room(Game *game, Room *r);
int remove_room(Game *game, int id);
int switch_room(Game *game, int roomId);

#endif // GAME_H