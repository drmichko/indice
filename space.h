#ifndef SPACE_H
#define SPACE_H
#define MAX 8

typedef struct listspace{
	int  p;
	int  V[  MAX   ];
	int  sp[ 256  ];
	int  a;
        struct listspace * next;
} enrspace, *listspace;



listspace spaces( int l, int c , int affine );
#endif

