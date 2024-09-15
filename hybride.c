#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"


int test(boole a)
{
// compute property at level 0 : \sum_{u\in \fdm| wt(u)==m/2} a_u*a_{\bar u}
    int u, v = 0;
    int t = ffdimen / 2;
    ANFtoTT(a);
    for (u = 0; u < ffsize; u++) {

        int u_bar = u ^ (ffsize - 1);
        if (u < u_bar && weight(u) == t)
            v ^= a[u] & a[u_bar];       // & pour * dans F2
    }
    ANFtoTT(a);
    return v == 0;
}
void pxxx( FILE*dst, boole f)
{
int x,y,i,p;
unsigned char aux[ ffsize ];
for( x = 0; x < ffsize; x++ )
        aux[x] = f[x];

fprintf(dst, "\n\n");
p = 1;
xform( aux, ffsize);
if ( aux[0] )
        fprintf( dst, "1+" );
for( x = 1; x < ffsize ; x++)
   if ( aux[x] == 1 ) {
        i=0;
        y=x;
        if (p==0) fprintf(dst, "+"); else p=0;
        while (y>0)
              {
              if ( y & 1 ) fprintf(dst, "x_%d", i+1);
              i++;
              y=y>>1;
              }
        }
}

        
boole myloadBoole(FILE *src ) 
//-get the current boolean function
{ boole res = NULL;
  char line[1024];
  while (! feof (src) )
    {
      line[0] = '\0';
      fgets (line, 1024, src);
      printf("%s", line );
      switch (*line)
        {
        case '#':
          break;
        case 'a' :
          res =  strtoboole ( line );
          return res;
          break;
        default:;
        }
    }
    return NULL;
}

void randboole( boole f, int d )
{
boole r = getboole(  );
int x;
for( x = 0; x < ffsize; x++ )
        if ( weight(x) < d )
                r[ x ] = random() & 1;

ANFtoTT( r );

for( x = 0; x < ffsize; x++ )
	f[x] ^= r[x];

}

int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    option(argc, argv);

    
    int num = 0;

    int    DR[9]={ -2  };
    boole  SP[9]={ NULL };

    initdegrees( optT, optR );

    int k, d;

    srandom( time(NULL) + getpid() ) ;

    while ((f = loadBoole(src))) {
	if (  job == num % mode) {
	    k = degree( f );
	    int s = valuation( f );
	    int t;
	    for( t = 0; t < optX; t++ ) {
		randboole( f, s );
	    	d =  restriction( f );
		if ( d < 0 ) d++;
		if ( DR[ k ] < d ) {
			DR[ k ] = d;
			if ( SP[k] ) 
				free(  SP[ k ]  );
			SP[ k ] = getboolecpy( f ); 
		}
	    }
	}
	free(f);
	num++;
    }

    FILE *save;
    char fn[32];
    sprintf( fn, "hybride/line-%d-%d-%d.txt", optR, job, ffdimen );
    save = fopen( fn, "a" );
    fprintf( save, "%2d !", optR );
    for (d = 0; d <= ffdimen; d++)
        fprintf( save, "%3d :", DR[d]);
    for( d = 1; d <= ffdimen; d++ )
            if ( SP[d] ) {
                        fprintf(save, "\n#sample : k=%d dr=%d", d , DR[d] );
                        panf( save, SP[d] );
                        fprintf(save, "\n");
            }
    fclose( save );
    return 0;
}
