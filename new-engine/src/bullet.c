
#include "game.h"
#include "bullet.h"
#include "vector.h"
#include "serialize/buffer.h"

/** Bullet_create(x, y)
 *
 *      Creates a new bullet object but does NOT add it to our game yet.
 *      Use Handler_new_bullet(x,y)
 */
Bullet *Bullet_create(Vector position, float angle) {
    const Bullet bullet = {
            .position = position,
            .velocity = Vector_scale(Vector_new(cosf(angle), sinf(angle)), 8.0f),
            .timer = 0,
            .sprite = {},
    };
    const Sprite *const sprite = get_sprite(BULLET, Game_renderer);
    set_field_memory(bullet.sprite, sprite);
    
    Bullet *const heap_bullet = (Bullet *) malloc(sizeof(Bullet));
    memcpy(heap_bullet, &bullet, sizeof(Bullet));
    return heap_bullet;
}

void Bullet_update(Bullet *bullet) {
    bullet->timer++;

    bullet->x += bullet->vel_x;
    bullet->y += bullet->vel_y;
    // TODO: Collision Checking with players
}

void Bullet_render(Bullet *bullet) {

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
