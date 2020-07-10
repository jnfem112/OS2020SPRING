#!/bin/bash

echo "my testcase 3 (1 file, total 273 MB)"
./slave \
1 \
../output/my_output/video_$1.mp4 \
$1 \
127.0.0.1 > ../output/time_size/my_testcase_3_slave_$1

echo ""
echo "page descriptor :"
dmesg | grep master:
dmesg | grep slave:

echo "diff (video.mp4) :"
diff ../input/my_input/video.mp4 ../output/my_output/video_$1.mp4

rm -f temp
