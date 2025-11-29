#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"
#include <stdio.h>
#include "sprites.h"

/**
 * Initialize a game object (the base of any object to add in the game structure)
 * @param go A pointer to the game object to initialize
 * @return 0 ok; 1 Invalid parameters
 */
int init_gameobject(GameObject *go)
{
    if (go == NULL)
    {
        SDL_Log("Invalid parameters(%p)", go);
        return 1;
    }

    go->collision = (SDL_Rect) {0, 0, 0, 0};
    go->x = 0.f;
    go->y = 0.f;
    go->killed = 0;
    go->step = NULL;
    go->draw = NULL;
	go->free_obj = NULL;
    init_sprite(&go->sprite);

    return 0;
}

/**
 *  Frees all resources associated with a menu
 * @param go A pointer to the game object to free
 * @return 0 ok; 1 Invalid parameters; 2 Fail to free sprite
 */
int free_gameobject(GameObject *go)
{
    if (go == NULL)
    {
        SDL_Log("Invalid parameters(%p)", go);
        return 1;
    }

    go->x = 0;
    go->y = 0;
    go->killed = 0;
    if (&go->sprite)
    {
        if (free_sprite(&go->sprite) != 0)
        {
            SDL_Log("Fail to free sprite");
            return 2;
        }
    }
    if (go->step != NULL)
    {
        go->step = NULL;
    }

    if (go->draw != NULL)
    {
        go->draw = NULL;
    }
	free(go);
    return 0;
}

/**
 * Check if there is a meet point of 2 game objects (by rectangle sprite shape)
 * @param a The first game object
 * @param b The second game object
 * @return 0 No collision; 1 Collision detected; -1 Invalid parameters
 */
int is_collide(GameObject *a, GameObject *b)
{
    if (a == NULL || b == NULL)
    {
        return -1;
    }

    // Check  if there a inner join between the shapes
    if (a->x + a->collision.x < b->x + b->collision.x + b->collision.w &&
        a->x + a->collision.x + a->collision.w > b->x + b->collision.x &&
        a->y + a->collision.y < b->y + b->collision.y + b->collision.h &&
        a->y + a->collision.y + a->collision.h > b->y + b->collision.y)
    {
        // SDL_Log("x: %f", a->x + a->collision.x)
        return 1;
    }
    return 0;
}

Vector2D dir_to(const int start_x, const int start_y, const int target_x, const int target_y)
{
    Vector2D direction;
    float dx = target_x - start_x;
    float dy = target_y - start_y;
    float distance = sqrt(dx * dx + dy * dy);

    // Normaliser le vecteur de direction
    direction.x = dx / distance;
    direction.y = dy / distance;

    return direction;
}