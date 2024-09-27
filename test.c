#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

void test( boole f )
{
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0, count = 0;

    initdegrees( optT, optR );


 
    while ((f = loadBoole(src))) {
	if (  job == num % mode) { 
	    	int t = testRestriction( f );
       		if ( t ) count++;
		printf("\n %d / %d", count, num  ); 
	}
	free(f);
	num++;
    }
    printf("\n");
    return 0;
}
