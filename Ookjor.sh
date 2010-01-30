#!/bin/sh

HOME=$(pwd)/bin_src
echo $HOME
LD_LIBRARY_PATH=$HOME
export LD_LIBRARY_PATH

./bin_src/Ookjor
