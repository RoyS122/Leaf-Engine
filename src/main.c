#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "rooms.h"
#include "types.h"
#include "buttons.h"
#include "menus.h"
#include <windows.h>
#include "lua_engine.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

{
    freopen("error_log.txt", "w", stderr);
    fprintf(stderr, "Game started...\n");

    SDL_Log("Game started...");

    Game game;
    init_game(&game, 60, 800, 600);
    game.DisplayCollideBoxes = 1;
    Engine_game = &game;

    lua_State* L = luaL_newstate(); // crée un nouvel état Lua
    luaL_openlibs(L);               // ouvre les bibliothèques standard
    game.L = L;
    register_engine_api(L);         // <-- ici tu passes L

    
    // SDL_Log("MID %d", game.EnnemyNBR);
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        fprintf(stderr, "TTF_Init Error: %s", TTF_GetError());

        SDL_Quit(); // Assurez-vous de nettoyer SDL avant de quitter si l'initialisation échoue
        return 1;
    }
    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Jeu SDL2",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          game.WindowWidth,
                                          game.WindowHeight,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    game.renderer = renderer; 

    if (luaL_dofile(L, "game.lua") != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        printf("Erreur Lua : %s\n", err);
    }
    
    gameloop(&game);

    
    // Nettoyage

	free_game(&game);
    fflush(stderr);
    fclose(stderr);
	
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
