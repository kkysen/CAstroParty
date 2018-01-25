
#include "game.h"
#include "bullet.h"
#include "vector.h"

/** Bullet_create(x, y)
 *
 *      Creates a new bullet object but does NOT add it to our game yet.
 *      Use Handler_new_bullet(x,y)
 */
struct bullet *Bullet_create(Vector pos, float angle) {
    struct bullet *bullet = malloc( sizeof(struct bullet) );
    bullet->pos = pos;

    bullet->vel_x = 8.0f * cosf(angle);
    bullet->vel_y = 8.0f * sinf(angle);
    
    bullet->rect = malloc( sizeof(SDL_Rect) );

    return bullet;
}

void Bullet_update(struct bullet *bullet) {
    bullet->timer++;

    bullet->x += bullet->vel_x;
    bullet->y += bullet->vel_y;
    // TODO: Collision Checking with players
}

void Bullet_render(struct bullet *bullet) {

    SDL_SetRenderDrawColor(
            Game_renderer,
            255,
            255,
            0,
            255);

    bullet->rect->x = bullet->x - 8;
    bullet->rect->y = bullet->y - 8;
    bullet->rect->w = 16;
    bullet->rect->h = 16;
    SDL_RenderDrawRect(Game_renderer, bullet->rect);
}
