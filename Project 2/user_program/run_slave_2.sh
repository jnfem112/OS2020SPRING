#!/bin/bash

echo "sample testcase 2 (1 file, total 12 MB)"
./slave \
1 \
../output/sample_output_2/target_file_$1 \
$1 \
127.0.0.1 > ../output/time_size/sample_testcase_2_slave_$1

echo ""
echo "page descriptor :"
dmesg | grep master:
dmesg | grep slave:

echo ""
echo "diff (target_file) :"
diff ../input/sample_input_2/target_file ../output/sample_output_2/target_file_$1

rm -f temp
