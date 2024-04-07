#include <stdio.h>
#include <stdlib.h>
#include "space.h"
#include "boolean.h"

int count = 0;
int nbl = 4;
int nbc = 8;

void appendspace( int a, int V[] , listspace *lsp )
{ listspace aux;
  aux = calloc( 1, sizeof(enrspace) );
  aux->p = a;
  int i;
  for( i = 0; i < MAX; i++ )
  	aux->V[i] = V[i];
  aux->next = *lsp;

  int v = 0;
  int iter = 1;
  int limite  = 1 << nbl;
  while ( iter < limite ) {
                    int i = __builtin_ctz( iter );
                    a ^= aux->V[ i ];
                    v ^= 1 << i;
                    aux->sp[v] =  a;
                    iter++;
                }

  *lsp = aux;
  count++;
}



void echelon( int P, int *A, int l, int c, listspace *lsp )
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
	    for ( a = 0; a < ffsize ; a++ )
		   if( ( a & P ) == 0 )
			   appendspace( a ,  V  , lsp  );
	}
	return;
    }

    if (l < nbl) {
	A[c] = 1 << l;
	echelon(P + (1 << c ) , A, l + 1, c + 1, lsp);
    }

    for (x = 0; x < (1 << l); x++) {
	A[c] = x;
	echelon(P, A, l, c + 1, lsp);
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


listspace spaces(  int l, int c )
{   listspace lz = NULL;
    count = 0;
    nbl = l;
    nbc = c;
    int *A = calloc(nbc, sizeof(int));
    int P = 0;
    echelon(P, A, 0, 0,  &lz);
    printf("\n#number of [%d,%d]-spaces : %d\n", l, c , count );
    return lz;

}
