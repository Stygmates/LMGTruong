#!/bin/sh

sudo apt-get install libboost-all-dev

git clone https://github.com/assimp/assimp.git

cmake assimp/CMakeLists.txt -G 'Unix Makefiles'

make --directory assimp/