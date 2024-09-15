#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

#include "basistools.h"



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
    int cpt[3] = {0};
 
    while ((f = loadBoole(src))) {
	if (  job == num % mode) {
    		base.table = calloc(  base.size , 1 );
		d = restriction( f );
		cpt[ d + 1 ]++;
		d = Restriction( f, & base );
		//	printf("tour=%d\n",  d );
		free( base.table );
	}
	free(f);
	num++;
    }
    printf("#repartition : ");
    for( d = 0; d < 3; d++ )
	    printf(" %d", cpt[d] );
    printf("\n");
    return 0;
}
