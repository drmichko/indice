mode=72

while getopts "D:R:f:m:" opt
do
	case $opt  in
		D) dim=$OPTARG;;
		R) spc=$OPTARG;;
		f) file=$OPTARG;;
		m) mode=$OPTARG;;
	esac
done

start=$( date +%s )
for (( j  = 0; j < $mode; j++ )) ; do
	./calc.exe -R$spc  -D$dim  -m$mode -j$j   -f $file    & 
done

wait

ends=$( date +%s )

let run=ends-start
let total=run*mode

echo runtime : $run x $mode  = $total
