dim=$1
spc=$2
file=$3
mode=$4

rm -rf /tmp/restrict-*


for (( j  = 0; j < $mode; j++ )) ; do
	./restrict.exe    -D$1 -R$2 -K3  -m$mode -j$j   -f $file > /tmp/restrict-$$-$j.txt  &  
done

wait

cat /tmp/restrict-$$-* | grep '!' | grep -v '\-1' 


cat /tmp/restrict-$$-* | grep '!' | grep -v '\-1' | tr -d '!' \
	| awk -F : 'BEGIN{for(i=0; i < 8; i++) t[i] = 0}{for( i = 1;i<=NF;i++) t[i]+=$i }END{for(i=0; i <8; i++) printf(" &%d", t[i] ); }'
echo

