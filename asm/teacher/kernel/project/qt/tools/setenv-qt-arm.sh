#!/bin/bash
QTEDIR=/opt/arm-qt4
CROSS_COMPILER=/opt/toolchains/arm920t-eabi
PATH=$QTEDIR/bin:$PATH
LD_LIBRARY_PATH=$QTEDIR/lib:$LD_LIBRARY_PATH
CPLUS_INCLUDE_PATH=$CROSS_COMPILER/arm-angstrom-linux-gnueabi/include/c++:$CROSS_COMPILER/arm-angstrom-linux-gnueabi/include/c++/arm-angstrom-linux-gnueabi:$CPLUS_INCLUDE_PATH
