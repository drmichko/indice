#include <stdio.h>
#include <stdlib.h>
#include "space.h"
#include "boolean.h"

int count = 0;
int nbspaces = 0;
int nbl = 4;
int nbc = 8;

void appendspace( int a, int P, int V[] , listspace *lsp )
{ listspace aux;
  aux = calloc( 1, sizeof(enrspace) );
  aux->p = P;
  int i;
  for( i = 0; i < MAX; i++ )
  	aux->V[i] = V[i];
  aux->next = *lsp;

  int v = 0, x = 0 ;
  int iter = 1;
  int limite  = 1 << nbl;
  aux->sp[ 0 ] =  0;
  aux->a =  a;
  while ( iter < limite ) {
                    int i = __builtin_ctz( iter );
                    v  ^= aux->V[ i ];
                    x ^= 1 << i;
                    aux->sp[ x ] =  v;
                    iter++;
                }

  *lsp = aux;
  count++;
}



void echelon( int P, int *A, int l, int c, listspace *lsp, int affine )
{
    int x;
    int V[ MAX ]={0};
    if (c == nbc) {
	if (l == nbl) {
	    int i, j;
	    for( i = 0; i < nbl; i++ )
		    for( j = 0; j < nbc; j++ )
			    if ( A[j] & (1<<i) ) V[i] ^= 1 << j;
		int a;
	    if ( affine  ) {
	      for ( a = 0; a < ffsize ; a++ )
		   if( ( a & P ) == 0 )
			   appendspace( a , P,  V  , lsp  );
	    }
	    else 
		    appendspace( 0  ,  P ,  V  , lsp  );
	}
	return;
    }

    if (l < nbl) {
	A[c] = 1 << l;
	echelon(P + (1 << c ) , A, l + 1, c + 1, lsp, affine);
    }

    for (x = 0; x < (1 << l); x++) {
	A[c] = x;
	echelon(P, A, l, c + 1, lsp, affine );
    }
    A[c] = 0;
}

int length( listspace  lz )
{ int res = 0;
	while( lz ){
		lz = lz->next;
		res++;
	}
	return res;
}


listspace spaces(  int l, int c, int affine )
{   listspace lz = NULL;
    count = 0;
    nbl = l;
    nbc = c;
    int *A = calloc(nbc, sizeof(int));
    int P = 0;
    echelon(P, A, 0, 0,  &lz, affine  );
    nbspaces = count;
    return lz;

}
