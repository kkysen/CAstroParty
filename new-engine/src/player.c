/** player.c
 *
 *      A controllable player
 *
 */
#include "player.h"
#include "game.h"
#include "input_handler.h"
#include "util/sdl_utils.h"

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
    
    const float angle = deg2rad(player->angle * 90f);
    const float acceleration = player->acceleration;
    if (player->button_shoot) {
        player->x += acceleration * cosf(angle);
        player->y += acceleration * sinf(angle);
    }

    player->button_shoot_prev = player->button_shoot;
}

void Player_render(struct player *player) {

    SDL_SetRenderDrawColor(
            Game_renderer,
            255,
            0,
            0,
            255);

//    player->rect->x = player->x;
//    player->rect->y = player->y;
//    player->rect->w = 64;
//    player->rect->h = 64;
//    SDL_RenderDrawRect(Game_renderer, player->rect);
    
    const Vector position = Vector_new(player->x, player->y);
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
