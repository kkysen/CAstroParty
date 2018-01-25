cmake_minimum_required(VERSION 3.9)
project(CAstroParty C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
#        src/main.c
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
        src/client.c
        src/client.h
        src/reaction.c
        src/reaction.h
        src/game_packet.c
        src/game_packet.h
        src/networking.c
        src/networking.h
        src/game2.c
        src/game2.h
        src/game_client_connect.c
        src/game_client_connect.h
        src/serialize/game_serialization.c
        src/serialize/game_serialization.h
        src/serialize/serialization.c
        src/serialize/serialization.h
        src/serialize/player_serialization.c
        src/serialize/player_serialization.h
        src/serialize/players_serialization.c
        src/serialize/players_serialization.h
        src/serialize/buffer.c
        src/serialize/buffer.h
        src/serialize/game_state_serialization.c
        src/serialize/game_state_serialization.h
        src/client_init_input.c
        src/client_init_input.h
        src/game_server_init.c
        src/game_server_init.h
        src/util/arg_parse.c
        src/util/arg_parse.h
        new-engine/legacy/networking.c
        new-engine/src/client_handler.c
        new-engine/src/client_handler.h
        new-engine/src/game.c
        new-engine/src/game.h
        new-engine/src/input_handler.c
        new-engine/src/input_handler.h
        new-engine/src/networking.c
        new-engine/src/networking.h
        new-engine/src/object_handler.c
        new-engine/src/object_handler.h
        new-engine/src/player.c
        new-engine/src/player.h
        new-engine/src/server_handler.c
        new-engine/src/server_handler.h
        new-engine/src/textures.c
        new-engine/src/textures.h
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
