#!/bin/bash

echo "sample testcase 1 (10 file, total 24 KB)"
./slave \
10 \
../output/sample_output_1/target_file_1_$1 \
../output/sample_output_1/target_file_2_$1 \
../output/sample_output_1/target_file_3_$1 \
../output/sample_output_1/target_file_4_$1 \
../output/sample_output_1/target_file_5_$1 \
../output/sample_output_1/target_file_6_$1 \
../output/sample_output_1/target_file_7_$1 \
../output/sample_output_1/target_file_8_$1 \
../output/sample_output_1/target_file_9_$1 \
../output/sample_output_1/target_file_10_$1 \
$1 \
127.0.0.1 > ../output/time_size/sample_testcase_1_slave_$1

echo ""
echo "page descriptor :"
dmesg | grep master:
dmesg | grep slave:

echo ""
for((i=1;i<=10;i++))
do
	echo "diff (target_file_${i}) :"
	diff ../input/sample_input_1/target_file_${i} ../output/sample_output_1/target_file_${i}_$1
done

rm -f temp
