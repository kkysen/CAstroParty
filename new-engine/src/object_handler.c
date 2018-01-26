/** object_handler
 *      
 *      Main object handler: 
 *          Controlls and loops through every object in the game
 *          Lets you add new gameobjects that get added to the loop
 *
 */

#include "object_handler.h"
#include "client_handler.h"
#include "input_handler.h"
#include "player.h"
#include "bullet.h"
#include "textures.h"

#define MAX_NUM_PLAYERS 1000
#define MAX_NUM_BULLETS 6400

// Game Object lists. Initialize all to NULL
static Player *players[MAX_NUM_PLAYERS] = {0};
static Bullet *bullets[MAX_NUM_BULLETS] = {0};

static size_t num_players = 0;
static size_t num_bullets = 0;

/** ObjectHandler_init()
 *      Initialize every starting object in the game here
 *      as well as any handler variables
 */
void ObjectHandler_init() {
    // set them to NULL by = {0}
}

/** ObjectHandler_tick()
 *      Ticks all of our objects (called at every frame)
 */
void ObjectHandler_tick() {
    for (size_t i = 0; i < num_players; ++i) {
        Player *const player = players[i];
        if (Client_got_new_inputs) {
            Player_update_keys(player, Client_unpacked_inputs[player->server_index]);
        }
        if (i == Client_player_index) {
            player->button_turn = InputHandler_button_turn;
            player->button_shoot = InputHandler_button_shoot;
        }
        Player_update(player);
    
        const Vector center = player->sprite.center;
        const float kill_zone2 = Vector_norm2(center);
        const Vector player_position = player->position;
        for (size_t j = 0; j < num_bullets; ++j) {
            if (bullets[j]->player_server_index == player->server_index)
                continue;

            const Vector bullet_position = bullets[j]->position;
            if (Vector_in_radius2(player_position, bullet_position, kill_zone2)) {
                // kill player and bullet
                player->alive = false;
                bullets[j]->timer = BULLET_TIMER_TIMEOUT;
                break;
            }
        }

        if (!player->alive) {
            free(player);
            // move last player to this spot
            players[i] = players[--num_players];
            players[num_players] = NULL;
            i--;
        }
    }
    
    for (size_t i = 0; i < num_bullets; ++i) {
        Bullet *const bullet = bullets[i];
        Bullet_update(bullet);
        if (bullet->timer > BULLET_TIMER_TIMEOUT) {
            free(bullet);
            // move last bullet to this spot
            bullets[i] = bullets[--num_bullets];
            bullets[num_bullets] = NULL;
            i--; // repeat for this bullet
        }
    }
}

/** ObjectHandler_render()
 *      Renders all of our objects (called at every frame)
 */
void ObjectHandler_render() {
    for (size_t i = 0; i < num_players; ++i) {
        Player_render(players[i]);
    }
    
    for (size_t i = 0; i < num_bullets; ++i) {
        Bullet_render(bullets[i]);
    }
}

/** ObjectHandler_new_player(x, y, server_id)
 *      Creates a new player object AND adds it to our game.
 *      Use this to make new players
 */
Player *ObjectHandler_new_player(const Vector position, const int server_id) {
    if (num_players == MAX_NUM_PLAYERS) {
        printf("ERROR: Cannot create new player! Exceeded maximum player buffer size\n");
        return NULL;
    }
    
    Player *const player = Player_create(position, server_id);
    players[num_players++] = player;
    return player;
}

/** ObjectHandler_new_bullet(x, y)
 *      Creates a new bullet object AND adds it to our game.
 *      Use this to make new bullets
 */
Bullet *ObjectHandler_new_bullet(const Vector position, const float angle) {
    if (num_bullets == MAX_NUM_BULLETS) {
        printf("ERROR: Cannot create new bullet! Exceeded maximum player buffer size\n");
        return NULL;
    }
    
    Bullet *const bullet = Bullet_create(position, angle);
    bullets[num_bullets++] = bullet;
    return bullet;
}
