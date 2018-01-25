/** player.c
 *
 *      A controllable player
 *
 */
#include "player.h"
#include "game.h"
#include "input_handler.h"

#include <SDL2/SDL.h>
#include <stdbool.h>


/** Player_create(x, y)
 *
 *      Creates a new player object but does NOT add it to our game yet.
 *      Use Handler_new_player(x,y)
 */
struct player *Player_create(float x, float y, int server_index) {
    struct player *player = malloc( sizeof(struct player) );
    player->x = x;
    player->y = y;
    player->server_index = server_index;
    
    player->button_turn = false;
    player->button_shoot = false;

    player->rect = malloc( sizeof(SDL_Rect) );

    return player;
}

void Player_update(struct player *player) {
    if (player->button_shoot) {
        player->x += 2;
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

    player->rect->x = player->x;
    player->rect->y = player->y;
    player->rect->w = 64;
    player->rect->h = 64;
    SDL_RenderDrawRect(Game_renderer, player->rect);
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
