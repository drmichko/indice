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



int tfr[ 256 ],  sgn[ 256 ], K [ 256 ];

typedef struct list {
        int * fct;
        struct list * next;
} enrlist, *list;

list  lf[ 4 ] = {NULL}, result;


void freelist( list l )
{
while ( l ){
        list aux = l;
        l = l->next;
        free( aux->fct);
        free( aux );
}
}

int     countp = 0;
void  * rootp = NULL;

list* table;

typedef struct _node_ {
        int    val;
        vector key ;
} node_t;

static int nodecompare( const void *a,  const void *b)
{
    node_t* fa, *fb;
  fa = (node_t*) a;
  fb = (node_t*) b;
  if ( fa->key < fb->key ) return -1;
  if ( fa->key > fb->key ) return +1;


  return 0;
}

int nouvelle;

int searchkey( vector key,  void **rootp, int* count )
{
node_t  *ptr = malloc( sizeof(node_t) );
ptr->key = key;
const void *ret;
int value;
    ret  = tsearch((void *) ptr, rootp , nodecompare );
    if ( ret == NULL)
        exit(EXIT_FAILURE);
    if ((*(node_t **) ret)->key != ptr->key ) {
        value = (*(node_t**) ret)->val;
        free( ptr );
        nouvelle = 0;
        return value;
    };
    // it's new !
    ptr->val = *count;
    *count   = *count + 1;
    value = ptr->val;
    nouvelle = 1;
    return value;
}

int findkey( vector key,  void **rootp  )
{
node_t  *ptr = malloc( sizeof(node_t) );
ptr->key = key;
const void *ret;
    ret  = tfind((void *) ptr, rootp , nodecompare );
    if ( ret == NULL) return  -1;
    free( ptr );
    return (*(node_t**) ret)->val;
}




void freetable( list * t, int n )
{
int i;
for( i = 0; i < n; i++ )
        freelist( t[i] );
free( t) ;
}


vector  key( int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  vector X = 0;
  for( t = 0; t < 64; t++ )
	  f[ t ] = fct[ t ];
  Fourier( f,  64   );
  for( t = 0; t < 64 ; t++ )
	 if ( abs( f[t] ) == 8  ) 
		 putbit( t, X );
  return X;
}

void mktable( list l, int n )
{
table = calloc( n, sizeof(list) );
rootp  = NULL;
countp = 0;
while ( l ) {
	vector X = key( l->fct ); 
        int val = searchkey( X , &rootp, &countp );
        list aux = l;
        l = l-> next;
        aux ->next =  table[ val ];
        table[ val ] = aux;
}
printf("\n#countp : %d", countp );
}

void print( int f[], int q )
{
printf("\nfct:");
int t;
for( t = 0; t < q; t++ )
	printf(" %d", f[t] );
printf("\n");
}

void plist( list l, int q  )
{
while ( l ){
        list aux = l;
        l = l->next;
        print( aux->fct, q );
}
}

void push32( int f[] )
{ list aux;
  aux = malloc( sizeof(enrlist) );
  aux->fct = calloc( 32, sizeof(int ) ); 
  int t;
  for( t = 0; t < 32; t++ )
  	aux->fct[t] = f[ t ];
  aux->next = result;
  result = aux;
}

void push64( int f[] )
{ list aux;
  aux = malloc( sizeof(enrlist) );
  aux->fct = calloc( 64, sizeof(int ) );
  int t;
  for( t = 0; t < 64; t++ )
        aux->fct[t] = f[ t ];
  aux->next = result;
  result = aux;
}


int subsize = 128;

listspace lsp = NULL;


int soluce;
int accept32(  int v )
{
v = abs( v );
return  ( v == 0 || v == 4 || v == 8 || v == 12 || v == 16 );
}


int accept64(  int v )
{
v = abs( v );
return  ( v == 0 ||  v == 8 || v == 16 );
}

int admis32( int fct[] )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 32; t++ )
	  f[ t ] = fct[ t ];

  Fourier( f,  32   );
  
  for( t = 0; t < 32 ; t++ )
	 if ( ! accept32(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}


int admis64( int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 64; t++ )
	  f[ t ] = fct[ t ];
  Fourier( f,  64   );
  for( t = 0; t < 64 ; t++ )
	 if ( ! accept64(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}

int admis128( int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 64; t++ )
	  f[ t ] = fct[ t ];
  Fourier( f,  128  );
  for( t = 0; t < 128 ; t++ )
	 if ( f[t] && abs(  f[ t ] ) != 16  ) {
		 return 0;
	 }
  return 1;
}

list  mkblock ( int offset  )
{ 
  int t;
  int pos[ 32 ] ={ 0 }, nbp = 0;
  int left[ 64 ];
  for( t = 0; t < 32 ; t++ ){
	  if ( tfr[ t + offset  ] == 0 ) {
		  pos[ nbp++ ] = t ;
	  } else  left[t] = left[ t + 32 ] = tfr[ t + offset ]; 
  }

  soluce = 0;
  result = NULL;
  int i, p;
  for( p = 0; p < (1 << nbp) ; p++ ) {
     	for( i = 0; i < nbp; i++ ){
	     	if ( ( 1 <<i ) & p ) { 
			      left[ pos[ i ] ] = 1;
			      left[ pos[ i ] + 32 ] = -1;
		}
	     		else  {
				left[ pos[ i ] ] = -1;
			        left[ pos[ i ] + 32 ] = +1;
			}
     		}
     	if ( admis64( left )  )  {
			push32( left );
			soluce++;
		}
	}
printf("\nblock %d ( %d )\n", soluce, nbp  );
return result;
}

int check64( int offset, int fct []  )
{ int f[ 256 ] = { 0 };
  int t;
  for( t = 0; t < 64; t++ )
	  if( tfr[ t + offset ] )  f[ t ] = fct[ t ];
          else f[ t ] = - fct[ t ];

  Fourier( f,  64   );
  for( t = 0; t < 64 ; t++ )
	 if ( ! accept64(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}



void glue64( int offset, list ll, list lr)
{
list lx;
int fct[64];
soluce = 0;
result = NULL;

while ( ll ) {
	int t;
	for( t = 0; t < 32; t++ )
                        fct[t] = ll->fct[t];
	lx = lr;
	while ( lx ) {
		for( t = 0; t < 32; t++ )
			fct[t + 32 ] = lx->fct[t];
		if ( admis64( fct )  && check64(offset, fct) ) {
			push64( fct );
			soluce++;
		}
		lx = lx -> next;
	}
	ll = ll-> next;
}
printf("\nglue 64 : %d\n", soluce );
}

int glue128( list ll, int right[] )
{
int fct[ 128 ];
int res = 0;
int t;
	for( t = 0; t < 64 ; t++ )
               fct [t + 64 ] = right[ t ];

	while ( ll ) {
		for( t = 0; t < 64 ; t++ )
			fct[ t  ] = ll->fct[t];
		if ( admis128( fct ) ) res++;
		ll = ll-> next;
		}
  return res;
}


void final( int offset, list ll, list lr)
{
list lx;
int fct[64];
soluce = 0;
result = NULL;
while ( ll ) {
	int t;
	for( t = 0; t < 32; t++ )
                        fct[t] = ll->fct[t];
	lx = lr;
	while ( lx ) {
		for( t = 0; t < 32; t++ )
			fct[t + 32 ] = lx->fct[t];
		if ( admis64( fct )  && check64(offset, fct) ) {
			 vector X = key( fct );
                         int val = findkey( X , &rootp );
			 if ( val >= 0 )  
				 soluce+=  glue128( table[val],  fct );
			 
		}
		lx = lx -> next;
	}
	ll = ll-> next;
}
printf("\nglue 64 : %d\n", soluce );
}

int test( boole f )
{ 
  int t;
  for( t =  0; t < 128; t++ )
	  sgn[ t ] = f[ t ] ? -1 : 1 ;

  for( t =  0; t < 128; t++ )
	  tfr[ t ] = f[ t ] ? -1 : 1 ;
   Fourier( tfr, 128 );
   for( t = 0; t < 128; t++ )
	if ( tfr[ t ] != 0 ) 
		tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

     printf("\n");
     int q; 
   for ( q = 0; q < 4; q++ ) 
   	lf[ q ] =  mkblock( q * 32 );

    glue64( 0,  lf[ 0 ], lf[ 1 ] ); 
    freelist( lf[0] );
    freelist( lf[1] );
    list lg = result;
    int nb = soluce;
    mktable( lg, nb ); 
    final( 64, lf[ 2 ], lf[ 3 ] ); 
    printf("\nfinal=%d", soluce );
    freetable( table, nb );
    tdestroy( rootp, free );

    freelist( lf[2]) ;
    freelist( lf[3]) ;
    //freelist( lg );
    return soluce > 0 ;
}


int main(int argc, char *argv[])
{

    boole f;
    //FILE * src = fopen( "data/baby-1-6-6.dat", "r" );

    initboole( 7 );
    option(argc, argv);

    
    int num = 0, count = 0;



    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
		      assert( isnearbent( f ) == 1 ); 
		      if ( test( f ) ) {
			      count++;
		      }
    		      printf("\ncount=%d / %d\n", count, num );
	}
	free( f );
	num++;
    }
    printf("\ncount=%d / %d\n", count, num );
   
    return 0;
}
