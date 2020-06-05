#!/bin/sh

cd `dirname $0`

BUILD="build"

if [ ! -d $BUILD ]; then
  mkdir $BUILD
fi

find libstick show_pics -type f -name "*.c" -or -name "*.cpp" -or -name "*.h" | while read FILE
do
  clang-format -i -style=Google ${FILE}
done

cd $BUILD
cmake ..
make
