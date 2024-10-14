#include "boolean.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "space.h"
#include "option.h"
#include "degrees.h"

       #define _GNU_SOURCE         /* Consultez feature_test_macros(7) */
       #include <search.h>

#define  putbit( t, X ) ( X |= ( ((vector)1) << t ) ) 
#define  xorbit( t, X ) ( X ^= ( ((vector)1) << t ) ) 

int tfr [256];
int soluce = 0, subsize;
listspace lsp = NULL;

#define papi(x,y) { t = x + y; y = x - y; x = t; } 
void  fast64( int f[] )
{
for(int i = 0; i < 64;  i += 2 ){
	int t;
	papi( f[i  ], f[i+1] );
}	
for(int i = 0; i  < 64; i += 4 ){
	int t;
	papi( f[i  ], f[i+2] );
	papi( f[i+1], f[i+3] );
}	
for(int i = 0; i  < 64; i += 8 ){
	int t;
	papi( f[i  ], f[i+4] );
	papi( f[i+1], f[i+5] );
	papi( f[i+2], f[i+6] );
	papi( f[i+3], f[i+7] );
}	
for(int i = 0; i  < 64; i += 16 ){
	int t;
	papi( f[i  ], f[i+8] );
	papi( f[i+1], f[i+9] );
	papi( f[i+2], f[i+10] );
	papi( f[i+3], f[i+11] );
	papi( f[i+4], f[i+12] );
	papi( f[i+5], f[i+13] );
	papi( f[i+6], f[i+14] );
	papi( f[i+7], f[i+15] );
}
int j;
for(int i = 0; i  < 64; i += 32 )
	for( j = 0; j < 16 ; j++ ){
		int t;
		papi( f[i+j], f[i+j+16] );
}	

for( j = 0; j < 32 ; j++ ){
                int t;
                papi( f[ j ], f[ j+32] );

}
}

void  fast128( int f[] )
{
int l;
for(int i = 0; i < 128;  i += 2 ){
	int t;
	papi( f[i  ], f[ i + 1] );
}	
for(int i = 0; i  < 128  ; i += 4 ){
	int t;
	papi( f[i  ], f[i+2] );
	papi( f[i+1], f[i+3] );
}	
for(int i = 0; i  < 128; i += 8 ){
	int t;
	papi( f[i  ], f[i+4] );
	papi( f[i+1], f[i+5] );
	papi( f[i+2], f[i+6] );
	papi( f[i+3], f[i+7] );
}	
for(int i = 0; i  < 128; i += 16 ){
	int t;
	papi( f[i  ], f[i+8] );
	papi( f[i+1], f[i+9] );
	papi( f[i+2], f[i+10] );
	papi( f[i+3], f[i+11] );
	papi( f[i+4], f[i+12] );
	papi( f[i+5], f[i+13] );
	papi( f[i+6], f[i+14] );
	papi( f[i+7], f[i+15] );
}
for(int i = 0; i  < 128; i += 32 )
	for( int j = 0; j < 16 ; j++ ){
		int t;
		papi( f[i+j], f[i+j+16] );
}	

for( int j = 0; j < 64 ; j++ ){
                int t;
                papi( f[ j ], f[ j+64] );

}
}
void check( int f[] )
{
int x[128], y[128];
for( int i = 0; i < 128; i++ )
	x[i] =y[i] = f[i];
Fourier( x, 128 );
fast128( y  );
for( int i = 0; i < 126; i++ )
	assert( x[i] == y[i] );
}

int main(int argc, char *argv[])
{

    int iter, t,  f[128];
    for( t = 0; t < 128; t++ )
	    f[t] = random() & 1 ? -1 : 1;

    iter = 1 ;
    iter <<=  atoi( argv[2] );

    switch ( argv[1][0] ) {
    	case 'S' : while ( iter-- ) Fourier( f, 128 ); break;
    	case 'F' : while ( iter-- ) fast128( f ); break;
    	case 'T' : while ( iter-- ) check( f ); break;
    }

    return 0;
}
