#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * Initialize a Sprite
 * @param s A pointer to the sprite to draw
 * @return 0 ok; 1 Invalid parameters
 */
int init_sprite(Sprite *s)
{
    if (!s)
    {
        SDL_Log("Invalid parameters(%p)", s);
        return 1;
    }
    s->width = 0;
    s->height = 0;
    s->shape = (SDL_Rect){0, 0, 0, 0};
    s->srcRect = (SDL_Rect){0, 0, 0, 0};
    s->texture = NULL;
    s->animSpeed = 0;
    s->currentFrame = 0;
    s->totalFrames = 0;
    s->frameCounter = 0;
    return 0;
}
/**
 * Free a sprite
 * @param s A pointer to the sprite to draw
 * @return 0 ok; 1 Invalid parameters; 2 failed to destroy texture
 */
int free_sprite(Sprite *s)
{
    if (!s)
    {
        SDL_Log("Invalid parameters(%p)", s);
        return 1;
    }
    s->width = 0;
    s->height = 0;
    s->shape = (SDL_Rect){0, 0, 0, 0};
    s->srcRect = (SDL_Rect){0, 0, 0, 0};
    if (s->texture != NULL)
    {
        SDL_DestroyTexture(s->texture);
    }
    s->texture = NULL;
    s->animSpeed = 0;
    s->currentFrame = 0;
    s->totalFrames = 0;
    s->frameCounter = 0;
    return 0;
}


/**
 * Update state of a sprite (to animate it)
 * @param sprite a pointer to the sprite to update
 * @param frameRate the framerate you actualy update the sprite
 * @return 0 ok; 1 Invalid parameters;
 */
int update_sprite(Sprite *sprite, int frameRate)
{
    if (sprite == NULL || frameRate == 0)
    {
        SDL_Log("Invalid parameters(sprite: %p frameRate: %i)", sprite, frameRate);
        return 1;
    }
    SDL_Log("Sprite update: %d", sprite->frameCounter);
    sprite->frameCounter++;
    if (sprite->animSpeed != 0)
    {
        if (sprite->frameCounter >= frameRate / sprite->animSpeed)
        {
            sprite->currentFrame = (sprite->currentFrame + 1) % sprite->totalFrames;
            sprite->frameCounter = 0;
        }
    }
    SDL_Log("current frame %i", sprite->currentFrame);
    return 0;
}


/**
 * Draw a sprite with a renderer
 * @param renderer A pointer to SDL_Renderer to render the sprite
 * @param sprite A pointer to the sprite to draw
 * @return 0 ok; 1 Invalid parameters; 2 Fail of rendering
 */
int draw_sprite(SDL_Renderer *renderer, Sprite *sprite)
{
    if (renderer == NULL || sprite == NULL)
    {
        SDL_Log("Invalid parameters(renderer: %p sprite: %p)", renderer, sprite);
        return 1;
    }
    

    SDL_Rect srcRect;
    srcRect.x = sprite->currentFrame * sprite->width;
    srcRect.y = 0;
    srcRect.w = sprite->width;
    srcRect.h = sprite->height;

    if (SDL_RenderCopy(renderer, sprite->texture, &srcRect, &sprite->shape) != 0)
    {
        SDL_Log("Error on rendering: %c", SDL_GetError());
        return 2;
    };
    return 0;
}

int load_texture(Sprite *sprite, SDL_Renderer *renderer, const char *path) {

      if (sprite == NULL || renderer == NULL || path == NULL)
    {
        SDL_Log("Invalid parameters(sprite: %p renderer: %p path: %p)", sprite, renderer, path);
        return 1;
    }
    
    SDL_Surface *temp_surface = IMG_Load(path); // load png img
    if (!temp_surface)
    {
        SDL_Log("Erreur de chargement du sprite: %s", SDL_GetError());
        return 2;
    }

    sprite->texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

    SDL_FreeSurface(temp_surface);
    
    if (!sprite->texture)
    {
        SDL_Log("Erreur de création de la texture: %s", SDL_GetError());
		return 3;
    }
    SDL_Log("Sprite bien chargé");
    return 0; // ok
} 