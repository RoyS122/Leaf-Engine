#include "game.h"

#include "gameobjects.h"
#include "menus.h"
#include "rooms.h"
#include <SDL2/SDL_ttf.h>
/**
 * Initialize a game 
 * @param game A pointer to the game to initialize
 * @param framerate the fps targeted
 * @param window_width the horizontal size of the window (or of the game canva)
 * @param window_height the vertical size of the window (or of the game canva)
 * @return 0 ok; 1 Invalid parameters; 2 Fail to allocate memory
 */
int init_game(Game *game, int framerate, int window_width, int window_height)
{
	if(!game || framerate < 1 || window_width < 1 || window_height < 1) {
		SDL_Log("Invalid parameters");
		return 1;
	}
    game->FrameRate = framerate;
    game->FrameDelay = 1000 / framerate;
    game->WindowWidth = window_width;
    game->WindowHeight = window_height;

    game->DisplayCollideBoxes = 0;

    game->MenuArray = NULL;
    game->MenuNBR = 0;

    game->GameObjectArray = NULL;
    game->GameObjectNBR = 0;

    game->RoomArray=NULL;
    game->RoomNBR = 0;
    game->currentRoom=-1;

    game->menuOpen = 0;
    game->font = NULL;
    
	return 0;
}

/**
 *  Add a Menu to game
 * @param game A pointer to the game
 * @param go The Menu to add to the game
 * @return 0 ok; 1 Invalid parameters; 2 Fail to allocate memory
 */
int add_menu(Game *game, Menu menu)
{
	
	if(!game) {
		SDL_Log("Invalid parameters");
		return 1;
	}
	
    game->MenuArray = realloc(game->MenuArray, (game->MenuNBR + 1) * sizeof(Menu));
    if (game->MenuArray == NULL)
    {
        SDL_Log("Failed to allocate memory for new menu");
        return 2;
    }

    game->MenuArray[game->MenuNBR] = menu;

    game->MenuNBR++;
	return 0;
}

/**
 *  Remove a menu by id
 * @param game A pointer to the game
 * @param id The id of the menu to delete
 * @return 0 ok; 1 Invalid parameters; 2 Fail to reallocate memory
 */
int remove_menu(Game *game, int id)
{
    if (id < 0 || id >= game->MenuNBR)
    {
        SDL_Log("Menu %i does not exist.", id);
        return 1;
    }
	free_menu(&game->MenuArray[id]);
    for (int i = id; i < game->MenuNBR - 1; i++)
    {
        game->MenuArray[i] = game->MenuArray[i + 1];
    }

    game->MenuArray = realloc(game->MenuArray, (game->MenuNBR - 1) * sizeof(Menu));
	SDL_Log("End realloc Menu array");
    if (game->MenuArray == NULL && game->MenuNBR - 1 > 0)
    {
        SDL_Log("Failed to allocate memory for remove menu");
        return 2;
    }
	
    game->MenuNBR--;
	SDL_Log("End remove menu id: %i", id);
	return 0;
}

/**
* Create the texture of a text
* @param renderer Pointer to the renderer where will draw the text
* @param font Pointer to a TTF_Font
* @param text Constant pointer to char (the string of the text)
* @param color The color of the text
* @param x The x coordinate of the text
* @param y The y coordinate of the text
* @return void
*/
void createTextTextureAndRender(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y)
{
    // Créer une surface à partir du texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface)
    {
        SDL_Log("Unable to create text surface: %s", TTF_GetError());
        return;
    }

    // Créer une texture à partir de la surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
        return;
    }

    // Définir la position du texte
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

    // Afficher le texte
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer la surface et la texture
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

/**
* Create the texture of a text
* @param lastTime the last tick updated
* @param frameCount the counter of the frame from the lastTime
* @param displayedFPS the nbr of fps displayed
* @return 0 Ok
*/
int calculate_fps(Uint32 *lastTime, int *frameCount, int *displayedFPS)
{
    Uint32 currentTime = SDL_GetTicks();
    (*frameCount)++;

    if (currentTime > *lastTime + 1000)
    { // Mise à jour toutes les 1s
        *displayedFPS = *frameCount;
        *lastTime = currentTime;
        *frameCount = 0;
    }
	
	return 0;
}


/**
 *  Add a room to game
 * @param game A pointer to the game
 * @param r The room to add to the game
 * @return 0 ok; 1 Invalid parameters; 2 Fail to allocate memory
 */
int add_room(Game *game, Room *r)
{
    if (!game || !r)
    {
        SDL_Log("Invalid parameters: (game: %p)", game);
        return 1;
    }

    game->RoomArray = realloc(game->RoomArray, (game->RoomNBR + 1) * sizeof(*game->RoomArray));
    if (game->RoomArray == NULL)
    {
        SDL_Log("Failed to allocate memory for new game object");
        return 2;
    }
    r->CurrentGame = game;
    game->RoomArray[game->RoomNBR] = r;
    game->RoomNBR++;
    return 0;
};


/**
 *  Remove a gameobject by id
 * @param game A pointer to the game
 * @param id The id of the object to delete
 * @return 0 ok; 1 Invalid parameters; 2 Fail to reallocate memory
 */
int remove_room(Game *game, int id)
{
	SDL_Log("try to remove ID: %i", id);
    if (id < 0 || id >= game->RoomNBR || !game)
    {
        SDL_Log("Invalid parameters: (game: %p id: %i)", game, id);
        return 1;
    }
    game->RoomArray[id]->free_room(game->RoomArray[id]);
    for (int i = id; i < game->RoomNBR - 1; i++)
    {
        SDL_Log("try to mv: %i", i);
        game->RoomArray[i] = game->RoomArray[i + 1];
    }
	SDL_Log("after free: %i", id);

    game->RoomArray = realloc(game->RoomArray, (game->RoomNBR - 1) * sizeof(*game->RoomArray));
    if (game->RoomArray == NULL && game->RoomNBR - 1 > 0)
    {
        SDL_Log("Failed to reallocate memory after removing game object");
        return 2;
    }
	SDL_Log("after free1: %i", id);
    game->RoomNBR--;
	SDL_Log("after free2: %i", id);
    return 0;
};
/**
 *  Switch to another room of the game
 * @param game A pointer to the game
 * @param roomId The id of the room you want to switch (-1 to unload all room and have no one loaded)
 * @return 0 ok; 1 Invalid parameters; 2 Fail to free room 
 */
int switch_room(Game *game, int roomId) {
    if(!game || roomId >= game->RoomNBR || roomId < -1) return 1;
    if(game->currentRoom > -1) {
        if (free_room(game->RoomArray[game->currentRoom])) return 2;
    }
    if(roomId != -1){ 
        game->currentRoom = roomId;
        const Room *gameR = game->RoomArray[game->currentRoom];
        if (gameR->setup != NULL) {
            gameR->setup(gameR);
        }
    }else{
        game->currentRoom = -1;
    }
    

    return 0;
}
/**
 *  Add a gameobject to game
 * @param game A pointer to the game
 * @param go The gameobject to add to the game
 * @return 0 ok; 1 Invalid parameters; 2 Fail to allocate memory
 */
int add_gameobject(Game *game, GameObject *go)
{
    if (!game)
    {
        SDL_Log("Invalid parameters: (game: %p)", game);
        return 1;
    }

    game->GameObjectArray = realloc(game->GameObjectArray, (game->GameObjectNBR + 1) * sizeof(*game->GameObjectArray));
    if (game->GameObjectArray == NULL)
    {
        SDL_Log("Failed to allocate memory for new game object");
        return 2;
    }

    game->GameObjectArray[game->GameObjectNBR] = go;
    game->GameObjectNBR++;
    return 0;
};

/**
 *  Remove a gameobject by id
 * @param game A pointer to the game
 * @param id The id of the object to delete
 * @return 0 ok; 1 Invalid parameters; 2 Fail to reallocate memory
 */
int remove_gameobject(Game *game, int id)
{
	SDL_Log("try to remove ID: %i", id);
    if (id < 0 || id >= game->GameObjectNBR || !game)
    {
        SDL_Log("Invalid parameters: (game: %p id: %i)", game, id);
        return 1;
    }
    game->GameObjectArray[id]->free_obj(game->GameObjectArray[id]);
    for (int i = id; i < game->GameObjectNBR - 1; i++)
    {
        game->GameObjectArray[i] = game->GameObjectArray[i + 1];
    }
	SDL_Log("after free: %i", id);

    game->GameObjectArray = realloc(game->GameObjectArray, (game->GameObjectNBR - 1) * sizeof(*game->GameObjectArray));
    if (game->GameObjectArray == NULL && game->GameObjectNBR - 1 > 0)
    {
        SDL_Log("Failed to reallocate memory after removing game object");
        return 2;
    }
	SDL_Log("after free1: %i", id);
    game->GameObjectNBR--;
	SDL_Log("after free2: %i", id);
    return 0;
};
/**
 *  Free the attribute and free space allocated to game
 * @param game A pointer to the game to free
 * @return 0 ok; 1 Invalid parameters; 
 */
int free_game(Game *game) {
	if (!game) {
		SDL_Log("Invalid parameters(game: %p)", game);
		return 1;
	}
	game->FrameRate = 0;
    game->FrameDelay = 0;
    game->WindowWidth = 0;
    game->WindowHeight = 0;
   
	while(game->MenuNBR > 0){
		remove_menu(game, game->MenuNBR - 1);
	}
    game->MenuArray = NULL;
	SDL_Log("End removing menus");
  

	while(game->GameObjectNBR > 0){
		remove_gameobject(game, game->GameObjectNBR - 1);
	}
    game->GameObjectArray = NULL;
    while(game->RoomNBR > 0){
		remove_room(game, game->RoomNBR - 1);
	}
    game->GameObjectArray = NULL;

    game->currentRoom = 0;
    game->menuOpen = 0;

    lua_close(game->L);
	return 0;
}




int gameloop(Game *game) {
    SDL_Log("debut gameloop function:\n");
    int running = 1;
    SDL_Event event;

    Uint32 frameStart;
    int frameTime;
    int lastFrameEnnemySpawn = 0;
    int DelaySpawnEnnemy = game->FrameRate * 1;

    SDL_Color textColor = {255, 255, 255, 255};


    Uint32 lastTime = SDL_GetTicks();
    int frameCount = 0;
    int displayedFPS = 0;
    SDL_Log("debut while:\n");
    while (running)
    {
        frameStart = SDL_GetTicks(); // Obtenir le temps actuel
        // SDL_Log("debut while2:\n");
        while (SDL_PollEvent(&event))
        {
            // SDL_Log("Event type: %d", event.type);

            switch (event.type)
            {
            case SDL_QUIT:
                SDL_Log("test");
                running = 0;
                break;
            case SDL_KEYDOWN:
                if (game->menuOpen > -1)
                {
                    Menu *m = &(game->MenuArray[game->menuOpen]);
                    if(!m) {
                        break;
                    }
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_DOWN: // Naviguer vers le bas
                        m->selectedIndex = (m->selectedIndex + 1) % m->ButtonNBR;
                        break;

                    case SDLK_UP: // Naviguer vers le haut
					
                        m->selectedIndex = (m->selectedIndex - 1 + m->ButtonNBR) % m->ButtonNBR;
                        break;
                    case SDLK_RIGHT: // Naviguer vers la droite
                        m->selectedIndex = (m->selectedIndex + 1) % m->ButtonNBR;
                        break;

                    case SDLK_LEFT: // Naviguer vers la gauche
					
                        m->selectedIndex = (m->selectedIndex - 1 + m->ButtonNBR) % m->ButtonNBR;
                        break;


                    case SDLK_RETURN: // Valider un bouton
                    case SDLK_SPACE:
                    {
                        Button *b = m->buttons[m->selectedIndex];
                        // Button *b = &m->buttons[m->selectedIndex]; // Prendre l'adresse du bouton

                        if (b == NULL)
                        {
                            SDL_Log("Erreur: Le bouton à l'index %i est NULL !", m->selectedIndex);
                        }
                        else
                        {
                            SDL_Log("any test planting %p", b);
                            SDL_Log("Bouton sélectionné: %s (Adresse: %p)", b->label, &b);
							b->onUse(game);
                        }
                    }
                    // Ici, tu peux exécuter l'action liée au bouton sélectionné
                    break;

                    case SDLK_ESCAPE: // Quitter le menu
                        SDL_Log("Retour au menu précédent");
                        game->menuOpen = -1;
                        break;
                    }
                }
                break;
            }
        }
      
        
        

        if (game->menuOpen > -1)
        {
             if(!game->MenuNBR > 0) {
                game->menuOpen = -1;
               
             }else {
                SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
                SDL_RenderClear(game->renderer);

                draw_menu(&game->MenuArray[game->menuOpen], game->renderer);
      
                SDL_RenderPresent(game->renderer);
                continue;
             }
         
            
        }
      
      
        // SDL_Log("After menu render:\n");
        // Gestion des événements

        // step_player(&player, &game);
        if (game->currentRoom > - 1) {
            if(game->RoomArray[game->currentRoom]->step != NULL) {
                game->RoomArray[game->currentRoom]->step(game->RoomArray[game->currentRoom], game);
            }
        
        } 
        for (int i = 0; i < game->GameObjectNBR; i++)
        {
            if (game->GameObjectArray[i]->step)
            {
                game->GameObjectArray[i]->step(game->GameObjectArray[i], game);
            }
        }
        // SDL_Log("After game object step:\n");

        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);

        // draw_player(player, renderer);
        if (game->currentRoom > - 1) {
            if(game->RoomArray[game->currentRoom]->draw != NULL) {
                game->RoomArray[game->currentRoom]->draw(game->RoomArray[game->currentRoom], game->renderer);
            }
        } 
        for (int i = 0; i < game->GameObjectNBR; i++)
        {
            if (game->GameObjectArray[i]->draw)
            {
                game->GameObjectArray[i]->draw(game->GameObjectArray[i], game->renderer, game->FrameDelay);
            }
            if(game->DisplayCollideBoxes) {
                SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
                SDL_Rect coll =  game->GameObjectArray[i]->collision;
                coll.x +=  game->GameObjectArray[i]->x;
                coll.y +=  game->GameObjectArray[i]->y;
                SDL_RenderDrawRect(game->renderer, &coll);
                SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
            }
            
        }
		SDL_Log("After game object draw:\n");
		for(int i = game->GameObjectNBR - 1; i >= 0; i --) {
			if (game->GameObjectArray[i]->killed) {
				SDL_Log("try to kill %i", i);
				remove_gameobject(game, i);
			}
		}
        if (game->font != NULL) {
            TTF_SetFontSize(game->font, 24);
        }
       
    
        // char fpsText[32];
        // snprintf(fpsText, sizeof(fpsText), "FPS: %d", displayedFPS);
        // createTextTextureAndRender(game->renderer, game->font, fpsText, textColor, 10, 10);

        // char ennNBR[32];
        // snprintf(ennNBR, sizeof(ennNBR), "Ennemies: %d", game->GameObjectNBR - 1);
        // createTextTextureAndRender(game->renderer, game->font, ennNBR, textColor, 10, 40);

        SDL_RenderPresent(game->renderer);

        calculate_fps(&lastTime, &frameCount, &displayedFPS);
        // Calculer le temps écoulé et attendre si nécessaire

        frameTime = SDL_GetTicks() - frameStart; // Temps écoulé depuis le début de la frame
        if (frameTime < game->FrameDelay)
        {
            SDL_Delay(game->FrameDelay - frameTime); // Attendre le temps restant pour atteindre 60 FPS
        }

        // print_player(player);
        
    }
    return 0;
}