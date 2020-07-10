#!/bin/bash

touch temp
dmesg -c > temp

echo "my testcase 2 (8 file, total 18 MB)"
./master \
8 \
../input/my_input/Chapter\ 0.pdf \
../input/my_input/Chapter\ 1.pdf \
../input/my_input/Chapter\ 2.pdf \
../input/my_input/Chapter\ 3.pdf \
../input/my_input/Chapter\ 4.pdf \
../input/my_input/Chapter\ 5.pdf \
../input/my_input/Chapter\ 6.pdf \
../input/my_input/Chapter\ 7.pdf \
$1 > ../output/time_size/my_testcase_2_master_$1
