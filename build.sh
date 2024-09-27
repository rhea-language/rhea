#!/bin/bash
make clean
make -j$(nproc)
ls -l ./dist
