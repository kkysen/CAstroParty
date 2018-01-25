#include<SDL2/SDL.h>
#include<stdbool.h>

#ifndef D_INPUT_HANDLER
#define D_INPUT_HANDLER

// Hardcoded key inputs
#define INPUT_KEY_ACCELERATE SDLK_UP
#define INPUT_KEY_TURN_LEFT SDLK_LEFT
#define INPUT_KEY_SHOOT SDLK_SPACE

// Whether we're pressing the key currently
bool InputHandler_key_accelerate;
bool InputHandler_key_turn_left;
bool InputHandler_key_shoot;


void InputHandler_init();
// Utility variables, handled in game loop
void InputHandler_press_key(SDL_Keycode key);
void InputHandler_release_key(SDL_Keycode key);
/*void InputHandler_tick();

// User variables, to be used throughout the game
bool InputHandler_is_key_held(SDL_Keycode key);
bool InputHandler_is_key_pressed(SDL_Keycode key);
bool InputHandler_is_key_released(SDL_Keycode key);
*/

#endif
