#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

#include "basistools.h"

void bingauss( int r, int m)
{
	int cpt = 1;
	int k;
	for( k=0; k < r ; k++ )
		cpt *= (1 << m) - (1<<k );
	for( k=0; k < r ; k++ )
		cpt /= ( 1<< r  ) - ( 1<<k );
	printf("#space=%d %d", cpt, nbspaces );
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0;

    initdegrees( optT, optR );

    basis_t base;
    base = monomialBasis(2, 2, 7);
    
    int d;
    int cpt[4] = {0};
    int CPT[4] = {0};
    fprintf(dst, "\n#number of [%d,%d]-spaces : %d\n", optR, ffdimen , nbspaces  );

    bingauss( optR, ffdimen );  

    while ((f = loadBoole(src))) {
	if (  job == num % mode) {
    		base.table = calloc(  base.size , 1 );
		if ( verbe ) {
			countspace( cpt, f );
			if ( verbe > 1 ) {
				printf("\n#space : ");
    				for( d = 0; d <= 3 ; d++ )
	    				printf(" %d", cpt[d] );
				printf("\n");
			}
    			for( d = 0; d <= 3 ; d++ )
				CPT[d]+= cpt[d];
		}
		if ( optX ) 
			Restriction( f, &base );
		free( base.table );
	}
	free(f);
	num++;
    }
    if ( verbe ){
	   printf("\n#space : ");
    	for( d = 0; d <= 3 ; d++ )
	    	printf(" %d", CPT[d] );
   }
    return 0;
}
