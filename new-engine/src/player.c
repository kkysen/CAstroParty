/** player.c
 *
 *      A controllable player
 *
 */
#include "player.h"
#include "game.h"
#include "input_handler.h"
#include "util/sdl_utils.h"
#include "vector.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

static GameTexture next_texture = BLUE_PLAYER;

/** Player_create(x, y)
 *
 *      Creates a new player object but does NOT add it to our game yet.
 *      Use Handler_new_player(x,y)
 */
struct player *Player_create(float x, float y, int server_index) {
    p("callocing player");
    struct player *player = calloc(1, sizeof(struct player) );
    p("calloced player");
    pp(player);
    
    player->acceleration = 3;
    player->x = x;
    player->y = y;
    player->server_index = server_index;
    
    player->button_turn = false;
    player->button_shoot = false;
    
    player->vel_angle = 5;

    player->rect = malloc( sizeof(SDL_Rect) );
    
    player->sprite = get_sprite(next_texture, Game_renderer);
    next_texture = (next_texture + 1) % NUM_PLAYERS;

    return player;
}

void Player_update(struct player *player) {
    if (player->button_turn) {
        player->angle += player->vel_angle;
    }
    
    Vector position = player->position;
    Vector velocity = player->velocity;
    
    const float angle = deg2rad(player->angle - 90.0f);
    const float acceleration = player->acceleration;
    if (player->button_shoot) {
        position.x += acceleration * cosf(angle);
        position.y += acceleration * sinf(angle);
    }

    player->button_shoot_prev = player->button_shoot;
    
    Vector center = player->sprite->center;

    // performant, possibly branchless if optimized
    const Vector window = Vector_new(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    Vector copy = position;
    clamp(copy.x, center.x, window.x - center.x);
    
    if (game_tick % 10 == 0) {
        pv(copy);
    }
    
//    Vector_clamp(position,
//                 center.x, center.y,
//                 window.x - center.x, window.y - center.y);
    
    // copy back to Player*
    player->position = position;
    player->velocity = velocity;
}

void Player_render(struct player *player) {

    SDL_SetRenderDrawColor(
            Game_renderer,
            255,
            0,
            0,
            255);
    
    const Vector position = player->position;
    const Vector sprite_center = player->sprite->center;
    const SDL_Rect dest_rect = {
            .x = (int) (position.x - sprite_center.x),
            .y = (int) (position.y - sprite_center.y),
            .w = (int) sprite_center.x,
            .h = (int) sprite_center.y,
    };
//    SDL_Point center = Vector_as_SDL_Point(position);
    sdl_warn_perror(SDL_RenderCopyEx(
            Game_renderer,
            player->sprite->texture,
            NULL, // TODO change later
            &dest_rect,
            player->angle,
            NULL,
            SDL_FLIP_NONE
    ));
    SDL_RenderDrawRect(Game_renderer, &dest_rect);
}

/** Player_update_keys( player, accel, left, shoot );
 *      Updates the inputs within this player
 *
 *      Each player has its own inputs, and this lets us
 *      change the inputs in this player which makes
 *      it act upon those inputs in the "update" loop
 */
void Player_update_keys(struct player *player, struct Networking_unpacked_inputs inputs) {
    player->button_turn = inputs.is_turning;
    player->button_shoot = inputs.is_shooting;
}
