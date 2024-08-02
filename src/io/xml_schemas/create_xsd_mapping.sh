#!/bin/bash

# get input file
file=$1

xsd cxx-tree --std c++20 --hxx-suffix .h --cxx-suffix .cpp --generate-doxygen --generate-serialization $file
