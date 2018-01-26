
#include "game.h"
#include "bullet.h"
#include "vector.h"
#include "serialize/buffer.h"
#include "util/utils.h"
#include "util/sdl_colors.h"

/** Bullet_create(x, y)
 *
 *      Creates a new bullet object but does NOT add it to our game yet.
 *      Use Handler_new_bullet(x,y)
 */
Bullet *Bullet_create(const Vector position, const float angle) {
    const Vector direction = Vector_direction(angle);
    const Bullet bullet = {
            .position = position,
            .velocity = Vector_scale(direction, 8.0f),
            .timer = 0,
            .sprite = {},
    };
    Sprite *const sprite = get_sprite(BULLET, Game_renderer);
    sprite->border_color = YELLOW;
    sprite->angle = angle;
    set_field_memory(bullet.sprite, sprite);
    
    Bullet *const heap_bullet = (Bullet *) malloc(sizeof(Bullet));
    memcpy(heap_bullet, &bullet, sizeof(Bullet));
    return heap_bullet;
}

void Bullet_update(Bullet *bullet) {
    bullet->timer++;
    Vector_i_add(bullet->position, bullet->velocity);
}

void Bullet_render(Bullet *bullet) {
    Sprite_draw(&bullet->sprite, bullet->position, Game_renderer);
}
