set -v
set -e
cd build/
rm -rf ./*
cmake -G "MSYS Makefiles" ..
make -j$NUMBER_OF_PROCESSORS
