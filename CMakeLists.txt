cmake_minimum_required(VERSION 3.9)
project(CAstroParty C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        src/main.c
        src/player.h
        src/vector.h
        src/vector.c
        src/player.c
        src/game.c
        src/game.h
        src/util/sdl_colors.c
        src/util/sdl_colors.h
        src/util/sdl_utils.c
        src/util/sdl_utils.h
        src/util/hash.c
        src/util/hash.h
        src/util/io.c
        src/util/io.h
        src/util/random.c
        src/util/random.h
        src/util/sigaction.c
        src/util/sigaction.h
        src/util/sized_string.c
        src/util/sized_string.h
        src/util/stacktrace.c
        src/util/stacktrace.h
        src/util/string_builder.c
        src/util/string_builder.h
        src/util/string_utils.c
        src/util/string_utils.h
        src/util/utils.c
        src/util/utils.h
        src/players.c
        src/players.h
        src/bullet.c
        src/bullet.h
        src/bullets.c
        src/bullets.h
        src/obstacle.c
        src/obstacle.h
        src/obstacles.c
        src/obstacles.h
        src/textures.c
        src/textures.h
        )

add_executable(CAstroParty ${SOURCE_FILES})