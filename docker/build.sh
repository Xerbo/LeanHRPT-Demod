#!/bin/bash
cd /root
git clone https://github.com/quiet/libcorrect.git && cd libcorrect
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
make -j4
make install

cd /root/LeanHRPT-Demod
mkdir build && cd build
git config --global --add safe.directory /root/LeanHRPT-Demod
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
make -j4
make package
