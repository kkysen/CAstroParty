/** object_handler
 *      
 *      Main object handler: 
 *          Controlls and loops through every object in the game
 *          Lets you add new gameobjects that get added to the loop
 *
 */

#include "object_handler.h"
#include "networking.h"
#include "client_handler.h"
#include "input_handler.h"
#include "player.h"
#include "bullet.h"
#include "game.h"

#include<stdio.h>
#include<stdlib.h>

#define OBJECT_HANDLER_PLAYER_BUFFER_SIZE 10
#define OBJECT_HANDLER_BULLET_BUFFER_SIZE 64

// Game Object lists. Initialize all to NULL
struct player *ObjectHandler_players[OBJECT_HANDLER_PLAYER_BUFFER_SIZE];
struct bullet *ObjectHandler_bullets[OBJECT_HANDLER_BULLET_BUFFER_SIZE];

/** ObjectHandler_init()
 *      Initialize every starting object in the game here
 *      as well as any handler variables
 */
void ObjectHandler_init() {
    // Initialize all of our objects to NULL, so we can stop when we're looping
    int i;
    for(i = 0; i < OBJECT_HANDLER_PLAYER_BUFFER_SIZE; i++)
        ObjectHandler_players[i] = NULL;

    for(i = 0; i < OBJECT_HANDLER_BULLET_BUFFER_SIZE; i++)
        ObjectHandler_bullets[i] = NULL;
}

/** ObjectHandler_tick()
 *      Ticks all of our objects (called at every frame)
 */
void ObjectHandler_tick() {
    int i = 0;
    // Update all players
    while(i < OBJECT_HANDLER_PLAYER_BUFFER_SIZE) {
        struct player *current_player = ObjectHandler_players[i];
        if (current_player != NULL) {
            // If our client got new inputs, update them in the player
            if (Client_got_new_inputs) {
                Player_update_keys(current_player, Client_unpacked_inputs[ current_player->server_index ]);
            }

            // If we're dealing with our own player, update it's keyboard inputs
            if (i == Client_player_index) {
                current_player->button_turn =  InputHandler_button_turn;
                current_player->button_shoot =      InputHandler_button_shoot;
            }
            Player_update(current_player);
        }
        i++;
    }
    i = 0;
    while(i < OBJECT_HANDLER_BULLET_BUFFER_SIZE) {
        struct bullet *current_bullet = ObjectHandler_bullets[i];
        if (current_bullet != NULL) {
            Bullet_update(current_bullet);

            // Clean up our bullet after a certain time
            if (current_bullet->timer > BULLET_TIMER_TIMEOUT) {
                free(current_bullet);
                ObjectHandler_bullets[i] = NULL;
            }
        }
        i++;
    }
    
}

/** ObjectHandler_render()
 *      Renders all of our objects (called at every frame)
 */
void ObjectHandler_render() {
    int i = 0;
    // Render all players
    while(i < OBJECT_HANDLER_PLAYER_BUFFER_SIZE) {
        struct player *current_player = ObjectHandler_players[i];
        if (current_player != NULL)
            Player_render(current_player);
        i++;
    }

    i = 0;
    while(i < OBJECT_HANDLER_BULLET_BUFFER_SIZE) {
        struct bullet *current_bullet = ObjectHandler_bullets[i];
        if (current_bullet != NULL)
            Bullet_render(current_bullet);
        i++;
    }
}

/** ObjectHandler_new_player(x, y, server_id)
 *      Creates a new player object AND adds it to our game.
 *      Use this to make new players
 */
struct player *ObjectHandler_new_player(double x, double y, int server_id) {
    struct player *player = Player_create(x,y, server_id);

    int i = 0;
    while(ObjectHandler_players[i] != NULL) i++;

    if (i >= OBJECT_HANDLER_PLAYER_BUFFER_SIZE) {
        printf("ERROR: Cannot create new player! Exceeded maximum player buffer size\n");
        exit(-1);
    }

    ObjectHandler_players[i] = player;

    return player;
}

/** ObjectHandler_new_bullet(x, y)
 *      Creates a new bullet object AND adds it to our game.
 *      Use this to make new bullets
 */
struct bullet *ObjectHandler_new_bullet(double x, double y) {
    struct bullet *bullet = Bullet_create(x,y);
    
    int i = 0;
    while(ObjectHandler_bullets[i] != NULL) i++;

    if (i >= OBJECT_HANDLER_BULLET_BUFFER_SIZE) {
        printf("ERROR: Cannot create new bullet! Exceeded maximum player buffer size\n");
        exit(-1);
    }

    ObjectHandler_bullets[i] = bullet;

    return bullet;
}
