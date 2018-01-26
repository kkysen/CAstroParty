#include<SDL2/SDL.h>
#include<stdbool.h>

#ifndef D_INPUT_HANDLER
#define D_INPUT_HANDLER

// Hardcoded key inputs
#define INPUT_BUTTON_TURN SDL_BUTTON_RIGHT
#define INPUT_BUTTON_SHOOT SDL_BUTTON_LEFT

// Whether we're pressing the key currently
bool InputHandler_button_turn;
bool InputHandler_button_shoot;


void InputHandler_init();
// Utility variables, handled in game loop
void InputHandler_press_button(Uint8 key);
void InputHandler_release_button(Uint8 key);
/*void InputHandler_tick();

// User variables, to be used throughout the game
bool InputHandler_is_key_held(SDL_Keycode key);
bool InputHandler_is_key_pressed(SDL_Keycode key);
bool InputHandler_is_key_released(SDL_Keycode key);
*/

#endif
