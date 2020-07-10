#!/bin/bash

echo "my testcase 2 (8 file, total 18 MB)"
./slave \
8 \
../output/my_output/Chapter\ 0_$1.pdf \
../output/my_output/Chapter\ 1_$1.pdf \
../output/my_output/Chapter\ 2_$1.pdf \
../output/my_output/Chapter\ 3_$1.pdf \
../output/my_output/Chapter\ 4_$1.pdf \
../output/my_output/Chapter\ 5_$1.pdf \
../output/my_output/Chapter\ 6_$1.pdf \
../output/my_output/Chapter\ 7_$1.pdf \
$1 \
127.0.0.1 > ../output/time_size/my_testcase_2_slave_$1

echo ""
echo "page descriptor :"
dmesg | grep master:
dmesg | grep slave:

echo ""
for((i=0;i<=7;i++))
do
	echo "diff (Chapter ${i}.pdf) :"
	diff ../input/my_input/Chapter\ ${i}.pdf ../output/my_output/Chapter\ ${i}_$1.pdf
done

rm -f temp
