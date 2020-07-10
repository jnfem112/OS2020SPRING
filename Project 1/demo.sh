#!/bin/bash

touch temp

file=("TIME_MEASUREMENT" "FIFO_1" "PSJF_2" "RR_3" "SJF_4")
for((i=0;i<5;i++))
do
	sudo dmesg -c > temp
	echo "========================= ${file[i]}.txt ========================="
	./project1 < OS_PJ1_Test/${file[i]}.txt
	dmesg | grep Project1
done

rm -f temp
