#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"


int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0;
    int    H[9]={ 0 };

    initdegrees( optT, optR );

    int d;
 
    while ((f = loadBoole(src))) {
	if (  job == num % mode) {
	    	d =  restriction( f );
		H[ d + 1 ]++;
		}
	free(f);
	num++;
    }
    printf("\n");
    for( d = 0; d <= optR; d++ )
       printf("%4d", d - 1 ); 
    printf("\n");
    for( d = 0; d <= optR; d++ )
       printf("%4d", H[d] ); 
    printf("\n");
    return 0;
}
