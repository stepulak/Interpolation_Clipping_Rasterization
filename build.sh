#!/bin/bash

COMPILER="g++"
CFLAGS="-std=c++17 -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -pedantic -c"
LFLAGS="-lSDL2 -lSDL2_image"
DIRECTORY="src/"
CPP_FILES="*.cpp"
OBJ_FILES="*.o"
APP_NAME="icr-algo"

function build() {
    pushd $DIRECTORY

    for file in `ls $CPP_FILES`; do
        echo "Compiling '$file'..."

        $COMPILER $CFLAGS $file

        if [[ $? -ne 0 ]]; then
            echo "Compilation of '$file' failed..."
            echo "Exiting..."
            exit -1
        fi
    done

    echo "Linking..."

    $COMPILER `ls $OBJ_FILES` $LFLAGS -o $APP_NAME

    popd

    mv $DIRECTORY$APP_NAME .

    echo "Building finished!"
}

function clean() {
    echo "Cleaning..."

    rm $DIRECTORY$OBJ_FILES
}

build
clean