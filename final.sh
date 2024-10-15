#!/bin/bash

mode=40
debut=20
fin=30

start=$( date +%s )

date

echo loop from $debut to $fin for mode=$mode

for (( num=debut; num < fin ; num++ ))
do
   ./final.exe -D7  -f representatives.txt   -j$num -m$mode  > final-$num.txt   &
done


wait

ends=$( date +%s )

let run=ends-start
echo runtime=$run


