#!/bin/bash
#!/bin/bash
mode=8

start=$( date +%s )


for (( num=0; num < mode; num++ ))
do
   ./final.exe -D7  -f representatives.txt   -j$num -m$mode  > final-$num.txt   &
done


wait

ends=$( date +%s )

let run=ends-start
echo runtime=$run


