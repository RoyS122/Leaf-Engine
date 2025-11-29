// types.h
#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Déclarations anticipées
typedef struct Game Game;
typedef struct GameObject GameObject;
typedef struct Room Room;

#define MAX_BUTTONS 5 // The max number of buttons on a menu

typedef struct
{
    float x, y;
} Vector2D;

typedef struct
{
    int width, height;
    SDL_Rect shape;
    SDL_Rect srcRect;
    SDL_Texture *texture;
    int animSpeed, currentFrame, totalFrames, frameCounter;
} Sprite;

typedef struct {
    int x, y, width, height;
} CollideBox;

typedef struct GameObject
{
    float x, y;
    Sprite sprite;
    SDL_Rect collision;

    int killed;

    int (*step)(GameObject *go, Game *game); 
    int (*draw)(GameObject *go, SDL_Renderer *renderer);
    int (*free_obj)(GameObject *go);
} GameObject;

typedef struct
{
    TTF_Font *font;
    SDL_Rect rect;
    SDL_Color *rectColorUnselected, *rectColorSelected, *textColor;
    float sizeSelectionFactor;
    const char *label;
    int isSelected;
    void (*onUse)(struct Game *g);
} Button;

typedef struct
{
    Button **buttons;
	int ButtonNBR;
	
    int selectedIndex;
} Menu;

typedef struct Room {
    
    GameObject **GameObjectArray;
    int GameObjectNBR;

    Game *CurrentGame;

   
    int (*setup)(Room *r);
    int (*step)(Room *r, Game *game);  
    int (*draw)(Room *r, SDL_Renderer *renderer);
    int (*free_room)(Room *r);

} Room;

typedef struct Game
{

    int menuOpen;
    int WindowWidth;
    int WindowHeight;
    int FrameRate;
    int FrameDelay;

    lua_State *L; 

    SDL_Renderer *renderer;
    TTF_Font *font;

    // Player *player;

    Room **RoomArray;
    int RoomNBR;
    int currentRoom;


    GameObject **GameObjectArray;
    int GameObjectNBR;

    int DisplayCollideBoxes;

    Menu *MenuArray;
    int MenuNBR;
} Game;

#endif // TYPES_H
