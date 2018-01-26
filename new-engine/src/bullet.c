
#include "game.h"
#include "bullet.h"
#include "vector.h"
#include "serialize/buffer.h"
#include "util/utils.h"

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
    if (memcmp(sprite, &bullet.sprite, sizeof(bullet.sprite)) != 0) {
        printf("bullet.sprite not loaded correctly");
        exit(1);
    }
    
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
    
    pp(bullet);
    const Vector position = bullet->position;
    const Vector sprite_center = bullet->sprite.center;
    const SDL_Rect dest_rect = {
            .x = (int) (position.x - sprite_center.x),
            .y = (int) (position.y - sprite_center.y),
            .w = (int) sprite_center.x,
            .h = (int) sprite_center.y,
    };
    
    SDL_RenderDrawRect(Game_renderer, &dest_rect);
}
