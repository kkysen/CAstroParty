//
// Created by kkyse on 12/29/2017.
//

#include "reaction.h"

#include <SDL_events.h>

#include "util/sdl_utils.h"

void Reaction_example() {
    Reaction reaction = REACTION_IS_SHOOTING;
    
    const bool is_accelerating = Reaction_is_accelerating(reaction);
    assert(is_accelerating == false);
    
    const bool is_shooting = Reaction_is_shooting(reaction);
    assert(is_shooting == true);
    
    Reaction_set_is_turning(reaction, true);
    const bool is_turning = Reaction_is_turning(reaction);
    assert(is_turning == true);
}

// TODO are these the right keys
#define ACCELERATE_KEY SDLK_UP
#define TURN_KEY SDLK_RIGHT
#define SHOOT_KEY SDLK_SPACE

// stateful reaction, fields will get turned on and off in Reaction_poll()
static Reaction reaction = REACTION_NONE;

static Reaction key_to_reaction(const SDL_Keysym *const key) {
    switch (key->sym) {
        case ACCELERATE_KEY:
            return REACTION_IS_ACCELERATING;
        case TURN_KEY:
            return REACTION_IS_TURNING;
        case SHOOT_KEY:
            return REACTION_IS_SHOOTING;
        default:
            return REACTION_NONE;
    }
}

Reaction Reaction_poll() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                const SDL_KeyboardEvent key = event.key;
                const Reaction key_reaction = key_to_reaction(&key.keysym);
                if (key_reaction == REACTION_NONE) {
                    // don't do anything if wrong key => no reaction
                    break;
                }
                if (key.type == SDL_PRESSED) {
                    reaction |= key_reaction; // turn bit on
                } else if (key.type == SDL_RELEASED) {
                    reaction &= ~key_reaction; // turn bit off
                }
                break;
            }
            case SDL_QUIT:
                reaction |= REACTION_IS_TERMINATED;
                break;
            default:
                break;
        }
    }
    return reaction;
}
