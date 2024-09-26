#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

void pdistrib(  int f[], int n )
{ int x, * cpt = calloc( 2*n, sizeof(int) );
  for( x = 0; x < n ; x++)
	  cpt[ f[x] + n ] ++;
  for( x = 0; x < 2*n ; x++)
          if ( cpt[ x ] )
		  printf(" %d [%d]", cpt[x], x -n );
  printf("\n");

}
int test( boole f )
{
  int x, tfr[ ffsize ];
  boole g = getboole();
  for( x = 0; x < ffsize; x++ )
          tfr[x] = f[x] ? -1 : +1;
  Fourier( tfr, ffsize );
  for( x = 0; x < ffsize; x++ )
      tfr[x]  = ( tfr[x] == 0 );  
  for( x = 0; x < ffsize; x++ )
	  g[x] = tfr[x];
  printf("\ndegree=%d\n", degree( g) );
  free( g );  
  Fourier( tfr, ffsize );
  pdistrib( tfr, ffsize );
  return 1;
}

int qtest( boole f , int w )
{
	int u, v;
	int val = 0;
	for( u = 0; u < ffsize; u++ )
		for( v = u+1 ; v < ffsize+1; v++ )
			if ( ( u|v) == w ) 
				val ^= f[u] & f[v];
	return val == 0 ;
}


int agltest( boole f )
{
	int i, w = ffsize - 1;
	TTtoANF( f );
	int val = 1;
	for ( i = 0; i < ffdimen; i++ )
		val &= qtest( f, w ^ (1<<i) );
	TTtoANF( f );
	return val == 1 ;
}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0;
    int count = 0;

    initdegrees( optT, optR );


 
    while ((f = loadBoole(src))) {
	if (  job == num % mode) 
	    	test( f );
	free(f);
	num++;
    }
       printf("\n#count=%d / %d", count, num  ); 
    printf("\n");
    return 0;
}
