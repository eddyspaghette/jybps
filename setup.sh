#!/bin/bash

FILENAME="./boost_1_77_0.tar.gz"
LIB_PATH="/usr/include/boost_1_77_0"
LIB_NAME="boost_1_77_0"



remove() {
    read -p "remove $FILENAME? (y/N): " ans
    if [ $ans == "y" ]; then
        rm -f $FILENAME
        echo "deleted $FILENAME"
    fi
}

if [ $EUID -ne 0 ]; then
    echo "rerun script as sudo"
    exit 1
fi

if [ ! -f $FILENAME ] && [ ! -d $LIB_PATH ]; then
    echo "$FILENAME" was not found and $LIB_PATH not found
    exit 2
fi


if [ -d $LIB_PATH ] && [ -f $FILENAME ]; then
    echo "$LIB_PATH was found"
    remove
    exit 0
elif [ -d $LIB_PATH ]; then
    echo "$LIB_PATH was found"
fi

if [ -f $FILENAME ] && [ ! -d $LIB_PATH ]; then
    if tar --version >/dev/null 2>&1; then
        tar -xvf $FILENAME
        echo "moving $LIB_NAME to $LIB_PATH"
        mv $LIB_NAME /usr/include
        if [ -f "makefile" ]; then
            make
        fi
        remove
    else 
        echo "need tar to extract"
    fi
fi


