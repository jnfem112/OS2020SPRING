#!/bin/bash

echo "my testcase 1 (1 file, total 1.6 GB)"
./slave \
1 \
../output/my_output/ubuntu-16.04.5-desktop-amd64_$1.iso \
$1 \
127.0.0.1 > ../output/time_size/my_testcase_1_slave_$1

echo ""
echo "page descriptor :"
dmesg | grep master:
dmesg | grep slave:

echo ""
echo "diff (ubuntu-16.04.5-desktop-amd64.iso) :"
diff ../input/my_input/ubuntu-16.04.5-desktop-amd64.iso ../output/my_output/ubuntu-16.04.5-desktop-amd64_$1.iso

rm -f temp
