#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "rooms.h"
#include "types.h"
#include "buttons.h"
#include "menus.h"
#include "lua_engine.h"

// 1. On n'inclut windows.h que sur Windows
#ifdef _WIN32
    #include <windows.h>
#endif

// 2. Utiliser main standard au lieu de WinMain
// SDL2 s'occupe de faire la conversion en WinMain sur Windows automatiquement
int main(int argc, char* argv[])
{
    // Empêcher les warnings "unused parameter"
    (void)argc;
    (void)argv;

    // 3. freopen est risqué sur certains systèmes, on peut le protéger
#ifdef _WIN32
    freopen("error_log.txt", "w", stderr);
#endif

    fprintf(stderr, "Game started...\n");
    SDL_Log("Game started...");

    Game game;
    init_game(&game, 60, 800, 600);
    game.DisplayCollideBoxes = 1;
    Engine_game = &game;

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    game.L = L;
    register_engine_api(L);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

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
        SDL_Log("Erreur Lua : %s", err); // SDL_Log est plus portable que printf
    }

    gameloop(&game);

    // Nettoyage
    free_game(&game);

#ifdef _WIN32
    fflush(stderr);
    fclose(stderr);
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
