#!/bin/bash
make clean
make -j$(nproc) zhivo-x86
make -j$(nproc) zhivo-cuda

ls -l ./dist
