#!/bin/bash

touch temp
dmesg -c > temp

echo "sample testcase 1 (10 file, total 24 KB)"
./master \
10 \
../input/sample_input_1/target_file_1 \
../input/sample_input_1/target_file_2 \
../input/sample_input_1/target_file_3 \
../input/sample_input_1/target_file_4 \
../input/sample_input_1/target_file_5 \
../input/sample_input_1/target_file_6 \
../input/sample_input_1/target_file_7 \
../input/sample_input_1/target_file_8 \
../input/sample_input_1/target_file_9 \
../input/sample_input_1/target_file_10 \
$1 > ../output/time_size/sample_testcase_1_master_$1
