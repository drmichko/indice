D=$1
n=$2

for (( r = D-1; r>=1; r-- )) ; do
	./search.exe  -D$1  -R$r  -m$2  $3     &
done

wait
echo done

