#!/bin/bash

touch temp

sudo dmesg -c > temp
echo "========================= TIME_MEASUREMENT.txt ========================="
./project1 < OS_PJ1_Test/TIME_MEASUREMENT.txt > output/TIME_MEASUREMENT_stdout.txt
dmesg | grep Project1 > output/TIME_MEASUREMENT_dmesg.txt

policy=("FIFO" "SJF" "PSJF" "RR")
for((i=0;i<4;i++))
do
	for ((j=1;j<=5;j++))
	do
		sudo dmesg -c > temp
		echo "========================= ${policy[i]}_${j}.txt ========================="
		./project1 < OS_PJ1_Test/${policy[i]}_${j}.txt > output/${policy[i]}_${j}_stdout.txt
		dmesg | grep Project1 > output/${policy[i]}_${j}_dmesg.txt
	done
done

rm -f temp
