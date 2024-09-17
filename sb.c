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
    int count = 0;

    initdegrees( optT, optR );


 
    while ((f = loadBoole(src))) {
	if (  job == num % mode) {
	    	if ( isnearbent( f ) ) count++;
		printf("%d\n", linearity(f) );
		}
	free(f);
	num++;
    }
       printf("\ncount=%d / %d", count, num  ); 
    printf("\n");
    return 0;
}
