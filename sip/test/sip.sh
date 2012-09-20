#!/bin/bash

make
cd msg
./register.sh
cd ..
./sip-call

