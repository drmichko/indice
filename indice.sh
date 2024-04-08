dim=$1
spc=$2
file=$3
mode=40

for j in {0..39} ; do
	./normal.exe -t$spc   -D$1 -m40 -j$j   -f $file > /tmp/norm-$$-$j.txt &  
done

wait

cat /tmp/norm-$$-*.txt | grep :  | grep -v '#' | sort | uniq > /tmp/lines.txt

cat /tmp/lines.txt

for(( i = 1; i <= $dim; i++ ))
do
	echo -n ' ' $i
done

echo

for(( i = 1; i <= $dim; i++ ))
do
	val=$( awk -F : "BEGIN{max=-2}{ if ( \$$i > max) max=\$$i }END{ print(max) }"   /tmp/lines.txt )
	echo -n ' ' $val
 done
echo

