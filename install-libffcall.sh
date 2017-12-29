mkdir libffcall
cd libffcall

wget https://ftp.gnu.org/gnu/libffcall/libffcall-2.0.tar.gz
tar -xzf libffcall-2.0.tar.gz
cd libffcall-2.0

mkdir builddir
cd builddir

../configure --cache-file=config.cache
make
make check
sudo make install

cd ../../..