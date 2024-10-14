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


#include "fft.c"


short int tfr[ 256 ];

typedef struct list {
        short int * fct;
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


vector  key( short int fct []  )
{ short int f[ 256 ] = { 0 };
  short int t;
  vector X = 0;
  for( t = 0; t < 64; t++ )
	  f[ t ] = fct[ t ];
  fast64( f );
  for( t = 0; t < 64 ; t++ )
	 if ( abs( f[ t ] ) == 8  ) 
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

void push32( short int f[] )
{ list aux;
  aux = malloc( sizeof(enrlist) );
  aux->fct = calloc( 32, sizeof( short int ) ); 
  short int t;
  for( t = 0; t < 32; t++ )
  	aux->fct[t] = f[ t ];
  aux->next = result;
  result = aux;
}

void push64( short int f[] )
{ list aux;
  aux = malloc( sizeof(enrlist) );
  aux->fct = calloc( 64, sizeof( short int  ) );
  short int t;
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

int admis32( short int fct[] )
{ short int f[ 256 ] = { 0 };
  short int t;
  for( t = 0; t < 32; t++ )
	  f[ t ] = fct[ t ];

  fft( f,  32   );
  int cpt = 0;
  for( t = 0; t < 32 ; t++ )
	 if ( ! accept32(  f[ t ] ) ) {
		 return 0;
	 if ( abs( f[t] ) == 16 ) cpt++;
	 if ( cpt > 1 ) return 0;
	 }
  return 1;
}


int admis64( short int fct []  )
{ short int f[ 256 ] = { 0 };
  short int t;
  for( t = 0; t < 64; t++ )
	  f[ t ] = fct[ t ];
  fast64( f );
  for( t = 0; t < 64 ; t++ )
	 if ( ! accept64(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}

int admis128( short int fct []  )
{ short int f[ 256 ] = { 0 };
  short int t;
  for( t = 0; t < 128; t++ )
	  f[ t ] = fct[ t ];
  fast128( f ) ;
  for( t = 0; t < 128 ; t++ )
	 if ( f[  t ] && abs(  f[ t ] ) != 16  ) {
		 return 0;
	 }
  return 1;
}

int complexity ( int q  )
{ 
  short int t;
  int nbp = 0;
  int offset = 32 * q;
  for( t = 0; t < 32 ; t++ ){
	  if ( tfr[ t + offset    ] == 0 ) 
		  nbp++;
  }
  return nbp;
}

list  mkblock ( int q  )
{ 
  short int t;
  short int pos[ 32 ] ={ 0 };
  int nbp = 0;
  short int left[ 64 ];
  int offset = 32 * q;
  for( t = 0; t < 32 ; t++ ){
	  if ( tfr[ t + offset    ] == 0 ) {
		  pos[ nbp++ ] = t ;
	  } else  left[t] = left[ t + 32 ] = tfr[ t + offset ]; 
  }

  int soluce = 0;
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
   printf(" %d ( %d ) ", soluce, nbp  );
   return result;
}

int check64( int offset, short int fct []  )
{ short int f[ 256 ] = { 0 };
  short int t;
  for( t = 0; t < 64; t++ )
	  if( tfr[ t + offset ] )  f[ t ] = fct[ t ];
          else f[ t ] = - fct[ t ];

  fast64( f );
  for( t = 0; t < 64 ; t++ )
	 if ( ! accept64(  f[ t ] ) ) {
		 return 0;
	 }
  return 1;
}



void glue64( list ll, list lr)
{
list lx;
short int fct[ 64 ];
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
		if ( admis64( fct )  && check64( 64 , fct) ) {
			push64( fct );
			soluce++;
		}
		lx = lx -> next;
	}
	ll = ll-> next;
}
printf("\nglue 64 : %d\n", soluce );
}

int check128( short int fct []  )
{ short int f[ 256 ] = { 0 };
  short int t;
  for( t = 0; t < 128; t++ )
      f[ t ] = fct[ t ];
  for( t = 128; t < 256; t++ )
	  if( tfr[ t ] )  f[ t  ] = tfr[t];
          else f[ t ] = - fct[ t-128 ];

  fft( f,  256   );
  for( t = 0; t < 256; t++ )
	 if ( abs(  f[ t ] ) != 16 ) 
		 return 0;
	 
  return 1;
}

int glue128( list ll,  short int right[] )
{
short int fct[ 128 ];
int res = 0;
short int t;
	for( t = 0; t < 64 ; t++ )
               fct [t + 64 ] = right[ t ];

	while ( ll ) {
		for( t = 0; t < 64 ; t++ )
			fct[ t  ] = ll->fct[t];
		if ( check128( fct )  ) res++;
		ll = ll-> next;
		}
  return res;
}


void final(  list ll, list lr)
{
list lx;
short int fct[64];
soluce = 0;
result = NULL;
int trial = 0;
while ( ll ) {
	short int t;
	for( t = 0; t < 32; t++ )
                        fct[t] = ll->fct[t];
	lx = lr;
	while ( lx ) {
		for( t = 0; t < 32; t++ )
			fct[t + 32 ] = lx->fct[t];
		if ( admis64( fct )  && check64( 64 , fct) ) {
			 vector X = key( fct );
                         int val = findkey( X , &rootp );
			 if ( val >= 0 ) {  
				 soluce+=  glue128( table[val],  fct );
				 trial++;
			 }
		}
		lx = lx -> next;
	}
	ll = ll-> next;
}
printf("\nglue 128 : %d (%d) \n", soluce, trial );
}

void prepare( boole f )
{ short int t;
  for( t =  0; t < 128; t++ )
          tfr[ t ] = f[ t ] ? -1 : 1 ;
   fast128( tfr );
   for( t = 0; t < 128; t++ )
        if ( tfr[ t ] != 0 )
                tfr[ t + 128 ] = tfr[ t ] = ( tfr[ t ] > 0 )   ? 1 : -1;

   int q;
   int cp[4];
   for ( q = 0; q < 4; q++ )
          cp[ q ] = complexity( q );

   if (  cp[0] + cp[1]  <= cp[2] + cp[3] ) {
   	printf("\ncp: %d %d block : ",   cp[0] + cp[1], cp[2]+cp[3] );
	return;
   }

   for( t =  0; t < 128; t++ )
          f[ t ] = f[ t ] ^ ( ( t & 64) > 0 ) ;

   assert( isnearbent( f ) );   

   for( t =  0; t < 128; t++ )
          tfr[ t ] = f[ t ] ? -1 : 1 ;

   fast128( tfr );

   for( t = 0; t < 128; t++ )
        if ( tfr[ t ] != 0 )
                tfr[ t + 128 ] = tfr[t] = ( tfr[ t ] > 0 )   ? 1 : -1;

   for ( q = 0; q < 4; q++ )
          cp[q] = complexity( q );

   printf("\ncp: %d %d block : ",   cp[0] + cp[1], cp[2]+cp[3] );
}

int test( boole f )
{ 

  
    prepare( f ); 
  

   for ( int q = 0; q < 4; q++ ) 
   	lf[ q ] =  mkblock( q );

    soluce = 0;
    	glue64(   lf[ 0 ], lf[ 1 ] ); 
    	freelist( lf[0] );
    	freelist( lf[1] );

    	list lg = result;
    	
	int nb = soluce;
    	
	mktable( lg, nb ); 
    	
	 final( lf[ 2 ], lf[ 3 ] ); 
    	
	 printf("\nfinal=%d", soluce );
    	freetable( table, nb );
    	tdestroy( rootp, free );
	freelist( lf[2]) ;
    	freelist( lf[3]) ;
    
    return soluce > 0 ;
}


int main(int argc, char *argv[])
{

    boole f;
    initboole( 7 );
    option(argc, argv);
    int num = 0, count = 0;
    while ((f = loadBoole(src))) {
		if (  job == num % mode) {
		      assert( isnearbent( f ) == 1 ); 
		      if (   test( f ) ) {
			      panf( stdout, f ); 
		      }
	}
	free( f );
	num++;
    }
    printf("\n#count=%d / %d\n", count, num );
   
    return 0;
}
