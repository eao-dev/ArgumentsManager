#!/bin/sh

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
SOURCE_DIR="$SCRIPT_DIR/Source/"
BUILD_DIR="$SCRIPT_DIR/build/"

BUILDTYPE=Release

if [ $1 == "debug" ]
then
	BUILDTYPE=Debug
fi

cmake -S $SOURCE_DIR -B $BUILD_DIR
cmake --build $BUILD_DIR --config $BUILDTYPE
echo $BUILDTYPE
read -p "..."