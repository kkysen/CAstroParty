/** networking.c
 *
 *  Networking variables and structs:
 *      - Holds input packets that are sent to the server
 *      - Holds the data packets that are sent to the clients
 *        (of every sendable object in the game)
 */

#include<stdbool.h>

// Inputs are sent through a SHORT variable and are BIT SHIFTED so that
// each input is held in a bit

// Object data is held in a STRUCT

/** Networking_player_packet
*       Struct holding just the data that the server
*       needs to send about the player
*/
struct Networking_player_packet {
    float x; float y;
    float angle;
    char name[16];
};

/** Networking_object_packet
 *      Struct holding all object data
 *      To be used by the CLIENT to UNPACK the server data
 */
struct Networking_object_packet {
    struct *Networking_player_packet players; // TODO: When sending to our socket, make it vary in size!
    unsigned short players_count;
};

/** Networking_input_packet
 *      Struct holding all input data
 *      To be used by the SERVER to UNPACK client data
 */
struct Networking_input_packet {
    bool key_accelerate;
    bool key_turn_left;
    bool key_shoot_prev;
}

/** Networking_set_nth_bit( *value, index, setto )
 *      Sets the "index"th bit of "value" to "setto"
 *      Assumes that the short is 0 (00000000)
 */
void Networking_set_nth_bit(short *value, short index, bool setto) {
    *value |= (setto << index);
    //&value = (value & (~(1 << index))) | (setto << index);
}

/** Networking_read_nth_bit( value, index )
 *      Gives you the "index"th bit of a given short
 */
void Networking_read_nth_bit(short value, short index) {
    return (value >> index) & 1;
}

/** void Networking_client_make_input_packet()
 *      Constructs a packet for client inputs
 *
 */
short Networking_client_make_input_packet(bool key_accelerate, bool key_turn_left, bool key_shoot_prev) {
    short result = 0;
    Networking_set_nth_bit(&result, 0, key_accelerate);
    Networking_set_nth_bit(&result, 1, key_turn_left);
    Networking_set_nth_but(&result, 2, key_shoot_prev);
    return result;
}

/** Networking_server_make_object_packet( players, player_count )
 *      Makes a packet for all of the objects that is sent to the server
 * /
struct Networking_object_packet *Networking_server_make_object_packet(struct player **players, int player_count) {
    struct Networking_object_packet *result = malloc(sizeof(struct Networking_object_packet));

    result->players_count = player_count;
    result->players = malloc( player_count * sizeof(struct Networking_player_packet));

    // Go through all players, place them in our player list
    int i = 0;
    while(i < player_count) {
        result->players[i]->x = *players[i].x;
        result->players[i]->y = *players[i].y;
        result->players[i]->angle = *players[i].angle;
        result->
    }

    return result;
}
*/


/** Networking_server_make_object_packet( players, player_count )
 *      Makes a char array packet for all of the objects that is sent to the server
 *      This packet is ready to be sent to the clients
 */
char *Network_server_get_object_packet(struct player **players, int player_count) {
    int player_size = sizeof(struct Networking_player_packet);

    char *result = malloc(
          sizeof(player_count) 
        + player_count * player_size 
        );

    // Networking player container which we will reuse
    struct Networking_player_packet *player_packet = malloc(player_size);

    int offset = 0;

    // Copy over all of our player data to the char array
    int i = 0;
    for(; i < player_count; i++) {
        player_packet->x = *players[i].x;
        player_packet->y = *players[i].y;
        player_packet->angle=*players[i].angle;
        memcpy( &result[i * size], player_packet, player_size );
    }

    free(player_packet);

    return result;
}

struct Networking_object_packet Network_client_convert_server_data(char *data, int data_size) {
    
}
