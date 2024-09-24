#!/bin/bash
#!/bin/bash
mode=$1

if [ -z "$mode"   ]; then
        echo mode ?
        exit
fi

rm  -f sieve-*.txt

start=$( date +%s )

for (( num=0; num< mode; num++ ))
do
   ./sieve.exe -R4 -D7  -f ~/boole/data/B-3-4-7.dat  -X1  -j$num -m$mode  -d sieve-$num.txt   &
done


wait

ends=$( date +%s )
let run=ends-start

cat sieve*.txt > abnormal.txt

echo run=$run
