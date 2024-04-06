#ifndef SPACE_H
#define SPACE_H
#define MAX 6
typedef struct listspace{
	int  p;
	int  V[ MAX    ];
	int  sp[ 64  ];
        struct listspace * next;
} enrspace, *listspace;



listspace spaces( int l, int c );
#endif

