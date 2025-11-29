#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"
#include <stdio.h>
#include "buttons.h"

#include <SDL2/SDL_ttf.h>
/**
* Draw a menu into a renderer
* @param m A pointer to the menu to draw
* @param renderer A pointer to the renderer
* @return 0 Ok; 1 Fail to draw a button;
*/
int draw_menu(Menu *m, SDL_Renderer *renderer)
{
    for (int i = 0; i < m->ButtonNBR; i++)
    {
        if (m->buttons[i] != NULL)
        {
			if (m->selectedIndex == i) {
				m->buttons[i]->isSelected = 1;
			}else{
				m->buttons[i]->isSelected = 0;
			}
            if (draw_button(m->buttons[i], renderer) != 0)
            {
				SDL_Log("Fail to draw button id: %i", i);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * Initialize a menu
 * @param m Pointer to the menu structure
 */
void init_menu(Menu *m)
{
    m->ButtonNBR = 0;
	m->buttons = NULL;
    m->selectedIndex = -1;
}

/**
 * Adds a button to the first available slot in the menu
 * @param m Pointer to the menu structure
 * @param b Pointer to the button to add
 * @return 0 on success, 1 Invalid parameters, 2 for invalid parameters
 */
int add_button(Menu *m, Button *b)
{
  if (!m || !b)
    {
        SDL_Log("Invalid parameters: (m: %p, b: %p)", m, b);
        return 1;
    }

    m->buttons = realloc(m->buttons, (m->ButtonNBR + 1) * sizeof(*m->buttons));
    if (m->buttons == NULL)
    {
        SDL_Log("Failed to allocate memory for new game object");
        return 2;
    }

	m->buttons[m->ButtonNBR] = b;
    m->ButtonNBR++;
    return 0;
}

/**
 *  Remove a button from a menu by id
 * @param m A pointer to the menu
 * @param id The id of the button to delete
 * @return 0 ok; 1 Invalid parameters; 2 Fail to reallocate memory
 */
int remove_button(Menu *m, int id)
{
	SDL_Log("try to remove ID: %i", id);
    if (id < 0 || id >= m->ButtonNBR || !m)
    {
        SDL_Log("Invalid parameters: (menu: %p id: %i)", m, id);
        return 1;
    }
	free_button(m->buttons[id]);
    for (int i = id; i < m->ButtonNBR - 1; i++)
    {
        m->buttons[i] = m->buttons[i + 1];
    }
	SDL_Log("after free: %i", id);

    m->buttons = realloc(m->buttons, (m->ButtonNBR - 1) * sizeof(*m->buttons));
    if (m->buttons == NULL && m->ButtonNBR - 1 > 0)
    {
        SDL_Log("Failed to reallocate memory after removing game object");
        return 2;
    }
	SDL_Log("after free1: %i", id);
	m->ButtonNBR --;
	SDL_Log("after free2: %i", id);
    return 0;
};

/**
 * Frees all resources associated with a menu
 * @param m Pointer to the menu to free
 * @return 0 ok; 1 invalid parameter; 2 Fail to free a button
 */
int free_menu(Menu *m)
{
    if (!m)
    {
        SDL_Log("Invalid parameters: %p", m);
        return 1;
    }

    // Free all buttons in the menu
	
  while(m->ButtonNBR > 0) {
	remove_button(m, m->ButtonNBR - 1);
  }

    // Reset menu state
    m->selectedIndex = -1;
	SDL_Log("test end free menu");
    return 0;
}