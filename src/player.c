/** player.c
 *
 *      A controllable player
 *
 */
#include "player.h"
#include "game.h"
#include "util/sdl_utils.h"
#include "bullet.h"
#include "serialize/buffer.h"
#include "object_handler.h"
#include "util/sdl_colors.h"
#include "textures.h"

#define PLAYER_MAX_SPEED 4

static GameTexture next_texture = BLUE_PLAYER;

Vector Player_direction(Player *const player) {
    return Vector_direction(player->angle);
}

/** Player_create(x, y)
 *
 *      Creates a new player object but does NOT add it to our game yet.
 *      Use Handler_new_player(x,y)
 */
Player *Player_create(const Vector position, const size_t server_index) {
    const Player player = {
            .position = position,
            .velocity = Vector_new(0, 0),
            .acceleration = 0.3,
            .server_index = server_index,
            .angle = 180,
            .angular_velocity = 5,
            .button_turn = false,
            .button_shoot = false,
            .button_shoot_prev = false,
            .sprite = {},
            .alive = true,
    };
    Sprite *const sprite = get_sprite(next_texture, Game_renderer);
    sprite->border_color = RED;
    sprite->angle = player.angle;
    next_texture = (next_texture + 1) % NUM_PLAYER_TEXTURES;
    set_field_memory(player.sprite, sprite);
    
    Player *const heap_player = (Player *) malloc(sizeof(Player));
    memcpy(heap_player, &player, sizeof(Player));
    return heap_player;
}

void Player_update(Player *player) {
    if (player->button_turn) {
        player->angle += player->angular_velocity;
        player->sprite.angle = player->angle;
    }
    
    Vector position = player->position;
    Vector velocity = player->velocity;
    
    const Vector direction = Player_direction(player);
    const float acceleration = player->acceleration;
    Vector_i_mul_add(velocity, direction, acceleration);
    
    const float norm = Vector_norm(velocity);
    if (norm != 0) {
        const float scale = clamped_max(norm, PLAYER_MAX_SPEED) / norm;
        Vector_i_scale(velocity, scale);
    }
    
    Vector_i_add(position, velocity);
    
    Vector center = player->sprite.center;
    // performant, possibly branchless if optimized
    Vector_clamp(position, center, window_size);
    
    // copy back to Player*
    player->position = position;
    player->velocity = velocity;

    if(player->button_shoot && !player->button_shoot_prev){
        Vector bullet_position = Player_direction(player);
        Vector_i_scale(bullet_position, 64);
        Vector_i_add(bullet_position, player->position);
        // pass player_index
        Bullet *const bullet = ObjectHandler_new_bullet(bullet_position, player->angle, player->server_index);
    }
    
    player->button_shoot_prev = player->button_shoot;
}

void Player_render(Player *player) {
    Sprite_draw(&player->sprite, player->position, Game_renderer);
}

/** Player_update_keys( player, accel, left, shoot );
 *      Updates the inputs within this player
 *
 *      Each player has its own inputs, and this lets us
 *      change the inputs in this player which makes
 *      it act upon those inputs in the "update" loop
 */
void Player_update_keys(Player *player, struct Networking_unpacked_inputs inputs) {
    player->button_turn = inputs.is_turning;
    player->button_shoot = inputs.is_shooting;
}
