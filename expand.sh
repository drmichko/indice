#!/bin/bash

mode=40
extra=1

while getopts "s:e:m:x" opt
do
        case $opt  in
                s) debut=$OPTARG;;
                e) fin=$OPTARG;;
                m) mode=$OPTARG;;
		x) extra=$OPTARG;;
        esac
done



start=$( date +%s )

date

echo loop from $debut to $fin for mode=$mode

for (( num=debut; num < fin ; num++ ))
do
   if [ ! -f expand-$num-$mode.txt ]; then
  	 ./expand.exe -D7  -f representatives.txt -X$extra   -j$num -m$mode  > expand-$num-$mode.txt   &
   else
   	echo ls -lart expand-$num-$mode.txt 
   fi
done


wait

ends=$( date +%s )

let run=ends-start
echo runtime=$run


