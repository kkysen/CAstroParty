//
// Created by kkyse on 12/26/2017.
//

#include "textures.h"

#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "util/utils.h"
#include "util/sdl_utils.h"
#include "vector.h"
#include "util/sdl_colors.h"

const size_t NUM_PLAYER_TEXTURES = BULLET; // next enum after last player

static const char *const texture_dir = "src/textures";

static const char *const texture_filenames[] = {
        "blue.png",
        "green.png",
        "purple.png",
        "red.png",
        "bullet.png",
};

static Sprite sprites[arraylen(texture_filenames)] = {};

const char *get_texture_name(const GameTexture texture) {
    return texture_filenames[texture];
}

SDL_Texture *get_texture(const GameTexture texture_index, SDL_Renderer *const renderer) {
    const Sprite *const sprite = get_sprite(texture_index, renderer);
    if (!sprite) {
        return NULL;
    }
    return sprite->texture;
}

void remove_texture(const GameTexture texture_index) {
    printf("Removing texture: %s\n", texture_filenames[texture_index]);
    Sprite *const sprite = sprites + texture_index;
    if (sprite->texture) {
        SDL_DestroyTexture(sprite->texture);
    }
    memset(sprite, 0, sizeof(Sprite));
}

Sprite *get_sprite(GameTexture texture_index, SDL_Renderer *renderer) {
    assert((size_t) texture_index < arraylen(sprites));
    if (sprites[texture_index].texture) {
        return sprites + texture_index;
    }
    destroy_all_textures_on_exit();
    const char *const path_parts[] = {texture_dir, texture_filenames[texture_index]};
    const char *const path = str_join("/", path_parts, arraylen(path_parts));
    SDL_Texture *const texture = IMG_LoadTexture(renderer, path);
    free((char *) path);
    if (!texture) {
        sdl_perror("IMG_LoadTexture(renderer, path)");
        return NULL;
    }
    int width;
    int height;
    if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) < 0) {
        sdl_perror("SDL_QueryTexture(texture, NULL, NULL, &width, &height) < 0");
    }
    const Sprite sprite = {
            .id = texture_index,
            .texture = texture,
            .border_color = BLACK,
            .width = width,
            .height = height,
            .center = Vector_new(width * 0.5f, height * 0.5f),
            .angle = 0,
    };
    memcpy(sprites + texture_index, &sprite, sizeof(Sprite));
    return sprites + texture_index;
}

void remove_sprite(const Sprite *const sprite) {
    remove_texture(sprite->id);
}

static void destroy_all_textures() {
    for (uint32_t i = 0; i < arraylen(sprites); ++i) {
        SDL_Texture *const texture = sprites[i].texture;
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }
    memset(sprites, 0, sizeof(sprites));
}

static bool will_destroy_all_textures_on_exit = false;

int destroy_all_textures_on_exit() {
    if (!will_destroy_all_textures_on_exit) {
        if (atexit(destroy_all_textures) != 0) {
            perror("atexit(destroy_all_textures)");
            return -1;
        }
        will_destroy_all_textures_on_exit = true;
    }
    return 0;
}

void Sprite_draw(const Sprite *const sprite, const Vector position, SDL_Renderer *const renderer) {
    const Vector sprite_center = sprite->center;
    const SDL_Rect dest_rect = {
            .x = (int) (position.x - sprite_center.x),
            .y = (int) (position.y - sprite_center.y),
            .w = (int) sprite_center.x,
            .h = (int) sprite_center.y,
    };
    
    sdl_warn_perror(SDL_RenderCopyEx(
            renderer,
            sprite->texture,
            NULL,
            &dest_rect,
            sprite->angle,
            NULL,
            SDL_FLIP_NONE
    ));
    
    SDL_SetRenderDrawColor(renderer, rgba_args(sprite->border_color));
    SDL_RenderDrawRect(renderer, &dest_rect);
}