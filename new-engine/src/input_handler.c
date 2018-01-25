/** input_handler
 *
 *      Handles our input.
 *      Now:
 *          Just holds input data
 *
 *      OLD:
 *          Use InputHandler_is_key_held, is_key_pressed, and is_key_released 
 *          to check for keyboard inputs
 *
 */

#include "input_handler.h"
#include "player.h"

#include <SDL2/SDL.h>
#include <stdbool.h>


// Previous keys (so we can tell whether we just pressed a key)
/*
bool InputHandler_key_accelerate_prev = false;
bool InputHandler_key_turn_left_prev = false;
bool InputHandler_key_shoot_prev = false;
*/

/** InputHandler_init()
 *      Initializes input handler, setting all variables to false
 */
void InputHandler_init() {
    InputHandler_key_accelerate = false;
    InputHandler_key_turn_left = false;
    InputHandler_key_shoot = false;
}


/** InputHandler_press_key( key )
 *      Sets a key to true, so that we can say it's being held
 *
 */
void InputHandler_press_key(SDL_Keycode key) {
    switch (key) {
        case INPUT_KEY_ACCELERATE:
            InputHandler_key_accelerate = true;
            break;
        case INPUT_KEY_TURN_LEFT:
            InputHandler_key_turn_left = true;
            break;
        case INPUT_KEY_SHOOT:
            InputHandler_key_shoot = true;
            break;
    }
}

/** InputHandler_release_key( key )
 *      Sets a key to false, so that we can say it's NOT being held
 *
 */
void InputHandler_release_key(SDL_Keycode key) {
    switch (key) {
        case INPUT_KEY_ACCELERATE:
            InputHandler_key_accelerate = false;
            break;
        case INPUT_KEY_TURN_LEFT:
            InputHandler_key_turn_left = false;
            break;
        case INPUT_KEY_SHOOT:
            InputHandler_key_shoot = false;
            break;
    }
}


/** InputHandler_tick()
 *      Tick (update) for input. Called once per frame, at the END of every frame.
 *      Sets our previous inputs so that we can detect 
 *      what our previous inputs were.
 * /
void InputHandler_tick() {
    InputHandler_key_accelerate_prev = InputHandler_key_accelerate;
    InputHandler_key_turn_left_prev = InputHandler_key_turn_left;
    InputHandler_key_shoot_prev = InputHandler_key_shoot;
}

// Is our key currently being held?
bool InputHandler_is_key_held(SDL_Keycode key) {
    switch (key) {
        case INPUT_KEY_ACCELERATE:
            return InputHandler_key_accelerate;
        case INPUT_KEY_TURN_LEFT:
            return InputHandler_key_turn_left;
        case INPUT_KEY_SHOOT:
            return InputHandler_key_shoot;
        default:
            return false;
    }
}

// Is our key pressed? (tapped once)
bool InputHandler_is_key_pressed(SDL_Keycode key) {
    switch (key) {
        case INPUT_KEY_ACCELERATE:
            return InputHandler_key_accelerate && !InputHandler_key_accelerate_prev;
        case INPUT_KEY_TURN_LEFT:
            return InputHandler_key_turn_left && !InputHandler_key_turn_left;
        case INPUT_KEY_SHOOT:
            return InputHandler_key_shoot && !InputHandler_key_shoot;
        default:
            return false;
    }
}

// Is our key released? (just let go)
bool InputHandler_is_key_released(SDL_Keycode key) {
    switch (key) {
        case INPUT_KEY_ACCELERATE:
            return !InputHandler_key_accelerate && InputHandler_key_accelerate_prev;
        case INPUT_KEY_TURN_LEFT:
            return !InputHandler_key_turn_left && InputHandler_key_turn_left;
        case INPUT_KEY_SHOOT:
            return !InputHandler_key_shoot && InputHandler_key_shoot;
        default:
            return false;
    }
}


*/

