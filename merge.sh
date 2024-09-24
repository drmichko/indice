dim=$1
direct=$2

cd $direct

table=table-$2-$1.txt

{
	for (( r = dim-1; r >=1; r-- ))
do
	grep -h '!' line-$r-*-$1.txt | tr '!' ':' > /tmp/line.txt
	awk -F ':' -vDIM=$dim  'BEGIN{DIM+=2;for( i=0; i <= DIM;i++)t[i]=-2;}\
		{for( i = 1; i <=NF; i++) if ( $i > t[i] ) t[i]=$i }\
			END{printf(" %d :", t[1]); for(i=2; i <= DIM ; i++) if ( t[i]!= -2) printf("&%3d ", t[i] ); else printf("&  - "); }' /tmp/line.txt
	echo
done
} > $table


echo  relative degree in dimension $dim

tr '&' ' ' < $table

