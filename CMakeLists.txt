cmake_minimum_required(VERSION 3.9)
project(CAstroParty C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        #        src/main.c
        old/src/player.h
        old/src/vector.h
        old/src/vector.c
        old/src/player.c
        old/src/game.c
        old/src/game.h
        old/src/util/sdl_colors.c
        old/src/util/sdl_colors.h
        old/src/util/sdl_utils.c
        old/src/util/sdl_utils.h
        old/src/util/hash.c
        old/src/util/hash.h
        old/src/util/io.c
        old/src/util/io.h
        old/src/util/random.c
        old/src/util/random.h
        old/src/util/sigaction.c
        old/src/util/sigaction.h
        old/src/util/sized_string.c
        old/src/util/sized_string.h
        old/src/util/stacktrace.c
        old/src/util/stacktrace.h
        old/src/util/string_builder.c
        old/src/util/string_builder.h
        old/src/util/string_utils.c
        old/src/util/string_utils.h
        old/src/util/utils.c
        old/src/util/utils.h
        old/src/players.c
        old/src/players.h
        old/src/bullet.c
        old/src/bullet.h
        old/src/bullets.c
        old/src/bullets.h
        old/src/obstacle.c
        old/src/obstacle.h
        old/src/obstacles.c
        old/src/obstacles.h
        old/src/textures.c
        old/src/textures.h
        old/src/client.c
        old/src/client.h
        old/src/reaction.c
        old/src/reaction.h
        old/src/game_packet.c
        old/src/game_packet.h
        old/src/networking.c
        old/src/networking.h
        old/src/game2.c
        old/src/game2.h
        old/src/game_client_connect.c
        old/src/game_client_connect.h
        old/src/serialize/game_serialization.c
        old/src/serialize/game_serialization.h
        old/src/serialize/serialization.c
        old/src/serialize/serialization.h
        old/src/serialize/player_serialization.c
        old/src/serialize/player_serialization.h
        old/src/serialize/players_serialization.c
        old/src/serialize/players_serialization.h
        old/src/serialize/buffer.c
        old/src/serialize/buffer.h
        old/src/serialize/game_state_serialization.c
        old/src/serialize/game_state_serialization.h
        old/src/client_init_input.c
        old/src/client_init_input.h
        old/src/game_server_init.c
        old/src/game_server_init.h
        old/src/util/arg_parse.c
        old/src/util/arg_parse.h
        src/client_handler.c
        src/client_handler.h
        src/game.c
        src/game.h
        src/input_handler.c
        src/input_handler.h
        src/networking.c
        src/networking.h
        src/object_handler.c
        src/object_handler.h
        src/player.c
        src/player.h
        src/server_handler.c
        src/server_handler.h
        src/textures.c
        src/textures.h
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
        src/util/string_builder.c
        src/util/string_builder.h
        src/util/string_utils.c
        src/util/string_utils.h
        src/util/utils.c
        src/util/utils.h
        src/util/arg_parse.c
        src/util/arg_parse.h
        src/serialize/buffer.c
        src/serialize/buffer.h
        src/serialize/serialization.c
        src/serialize/serialization.h
        src/vector.c
        src/vector.h
        src/bullet.c
        src/bullet.h
        src/util/stacktrace.c
        src/util/stacktrace.h
        )

add_executable(CAstroParty ${SOURCE_FILES})

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(CAstroParty
        ${SDL2_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIRS}
        )

target_link_libraries(CAstroParty
        ${SDL2_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        )