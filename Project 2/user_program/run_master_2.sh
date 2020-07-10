#!/bin/bash

touch temp
dmesg -c > temp

echo "sample testcase 2 (1 file, total 12 MB)"
./master \
1 \
../input/sample_input_2/target_file \
$1 > ../output/time_size/sample_testcase_2_master_$1
