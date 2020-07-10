#!/bin/bash

touch temp
dmesg -c > temp

echo "my testcase 3 (1 file, total 273 MB)"

wget https://www.dropbox.com/s/thy7fz963nzdxq9/video.mp4?dl=0 -O ../input/my_input/video.mp4

./master \
1 \
../input/my_input/video.mp4 \
$1 > ../output/time_size/my_testcase_3_master_$1
