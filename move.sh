#!/bin/bash

files=$(find -name *.cpp)


for file in $files; do
    name=$(echo $file | sed 's/cpp/cc/g')
    mv $file $name
done
