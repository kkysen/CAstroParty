#/bin/sh

sudo apt-get install libsdl2-2.0-0
sudo apt-get install libsdl2-dev

sudo apt-get install libsdl2-2.0-0
sudo apt-get install libsdl2-image-dev

TEST_FILE=sdl2_install_test.c

touch $TEST_FILE

echo "#include <SDL2/SDL.h>" >> $TEST_FILE
echo "#include <SDL2/SDL_image.h>" >> $TEST_FILE

gcc -E $TEST_FILE > /dev/null
rm $TEST_FILE
