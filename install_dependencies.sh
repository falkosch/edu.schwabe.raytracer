#!/usr/bin/env bash

mkdir -p build && (cd build && conan install .. --build missing)
