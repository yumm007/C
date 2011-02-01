#!/bin/sh
make distclean
source ../setenv-qt-arm.sh
qmake
make
