#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"
#include <stdio.h>
#include "game.h"
#include <SDL2/SDL_ttf.h>

/**
 * Adds a button to the first available slot in the menu
 * @param b Pointer to the button structure
 * @param renderer Pointer to the render where draw the button
 * @return 0 on success, 1 invalid parameter, 2 fail creating surface, 3 fail creating texture
 */
int draw_button(Button *b, SDL_Renderer *renderer)
{

    if (renderer == NULL || b == NULL)
    {
        SDL_Log("Invalid parameters (button: %p and renderer: %p)", b, renderer);
        return 1;
    }
	TTF_SetFontSize(b->font, (int)(28));
    // draw the button background
	SDL_Rect tmpRect = b->rect; 
	// SDL_Log("%i", b->isSelected);
	if(b->isSelected) {
		if(b->rectColorSelected) {
			SDL_Color *rc = b->rectColorSelected; 
			SDL_SetRenderDrawColor(renderer, rc->r , rc->g, rc->b, 255);
		}else{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
		tmpRect.w = (int)(tmpRect.w * b->sizeSelectionFactor);
		tmpRect.h = (int)(tmpRect.h * b->sizeSelectionFactor);
		 TTF_SetFontSize(b->font, (int)(28*b->sizeSelectionFactor));
	}else{
		if(b->rectColorUnselected) { 
			SDL_Color *rc = b->rectColorUnselected; 
			SDL_SetRenderDrawColor(renderer, rc->r , rc->g, rc->b, 255);
		}else{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}
	}
	
   
    SDL_RenderFillRect(renderer, &tmpRect);

    // get text size
    int textWidth = 0, textHeight = 0;
    TTF_SizeText(b->font, b->label, &textWidth, &textHeight);
   

    // create surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(b->font, b->label, *b->textColor);
    if (!textSurface)
    {
        SDL_Log("Unable to create text surface: %s", TTF_GetError());
        return 2;
    }

    // create texture
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
        SDL_FreeSurface(textSurface); // free surface if we cannot create the texture
        return 3;
    }

    // Get button position
    SDL_Rect textRect;
    textRect.x = b->rect.x + (tmpRect.w - textWidth) / 2;  // Centrer horizontalement
    textRect.y = b->rect.y + (tmpRect.h - textHeight) / 2; // Centrer verticalement
    textRect.w = textWidth;                                // Largeur du texte
    textRect.h = textHeight;                               // Hauteur du texte

    // draw button's text
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // free surface and texture
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    return 0;
}

/**
 * Initialize a button to add later in a menu
 * @param b Pointer to the button to initialize
 * @return 0 ok; 1 invalid parameters
 */
int init_button(Button *b)
{
    if (b == NULL)
    {
        SDL_Log("Invalid parameters(adress: %p)", b);
        return 1;
    }

    b->font = NULL;
    b->rect = (SDL_Rect){0, 0, 0, 0};
    b->rectColorUnselected = NULL;
    b->rectColorSelected = NULL;
    b->textColor = NULL;
    b->label = NULL;
    b->isSelected = 0;
    b->onUse = NULL;
    b->sizeSelectionFactor = 1;
    return 0;
}

/**
 * Free the content of a button
 * @param b Pointer to the button to free
 * @return 0 ok; 1 invalid parameters
 */
int free_button(Button *b)
{
	SDL_Log("try to free button" ); 
    if (!b)
    {
        SDL_Log("Invalid parameters(adress: %p)", b);
        return 1;
    }
		SDL_Log("try to free font" ); 
    if (b->font != NULL)
    {
		SDL_Log("Font pointer value: %p", b->font);
		SDL_Log("enter in font" ); 
        
        b->font = NULL;
    }
	SDL_Log("after free font" ); 
    b->rect = (SDL_Rect){0, 0, 0, 0};
    b->label = NULL;
    b->isSelected = 0;
    b->onUse = NULL;
    b->sizeSelectionFactor = (float)0;
	SDL_Log("try to free button end of 0 set" ); 
	free(b);
	SDL_Log("try to free button end of 1 set" ); 
    return 0;
}