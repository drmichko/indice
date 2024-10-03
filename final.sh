#!/bin/bash
#!/bin/bash
mode=40


start=$( date +%s )

echo prepare

./final.exe -R6 -D7  -f abnear.txt | grep anf > good.txt

ends=$( date +%s )
let run=ends-start

for (( num=0; num < mode; num++ ))
do
   ./final.exe -R7 -D7  -f good.txt -j$num -m$mode  > good-$num.txt   &
done


wait

ends=$( date +%s )
let run=ends-start

