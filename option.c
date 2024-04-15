#include "option.h"
#include "boolean.h"

int optr  = -1;
int tour  = 0;
int idt   = -1;
int iter  = 0;
int job   = 0;
int mode  = 1;
int verbe = 0;
int optT  = 0;
int optR  = 0;
int optK  = 0;
int dimen = 8;
FILE* src = NULL;
FILE* dst = NULL;

int option(int argc, char *argv[])
{

    initboole( 8 );
    int opt;
    while ((opt = getopt(argc, argv, "d:i:j:m:n:vt:r:f:iD:R:T:K:")) != -1) {
        switch (opt) {
        case 'i':
            iter = atoi(optarg);
            break;
        case 'j':
            job = atoi(optarg);
            break;
        case 'D':
	    dimen = atoi(optarg);
            initboole( dimen  );
            break;
        case 'm':
            mode = atoi(optarg);
            break;
        case 'K':
            optK  = atoi(optarg);
            break;
        case 'T':
            optT  = atoi(optarg);
            break;
        case 'R':
            optR = atoi( optarg );
            break;
        case 'r':
            optr = atoi(optarg);
            break;
        case 't':
            tour = atoi(optarg);
            break;
        case 'f':
            src = fopen(  optarg , "r" );
	    if ( ! src ) {
		    perror( optarg );
		    exit(1);
	    }
            break;
        case 'd':
            dst = fopen(  optarg , "w" );
	    if ( ! dst ) {
		    perror( optarg );
		    exit(1);
	    }
            break;
        case 'v':
            verbe++;
            break;
        case 'n':
            idt = atoi(optarg);
            break;
        default:                /* '?' */
            fprintf(stderr, "Usage: %s [-a anf ] [-m diemn ]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
   if ( verbe ){  
	   printf("m=%d", ffdimen );
   }
   return 1;
}
