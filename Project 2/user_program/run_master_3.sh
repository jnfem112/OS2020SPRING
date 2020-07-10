#!/bin/bash

touch temp
dmesg -c > temp

echo "my testcase 1 (1 file, total 1.6 GB)"

wget https://releases.ubuntu.com/16.04/ubuntu-16.04.6-desktop-amd64.iso -O ../input/my_input/ubuntu-16.04.5-desktop-amd64.iso

./master \
1 \
../input/my_input/ubuntu-16.04.5-desktop-amd64.iso \
$1 > ../output/time_size/my_testcase_1_master_$1
