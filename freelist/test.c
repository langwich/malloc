#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define MINSIZE				sizeof(Free_Header)
#define WORD_SIZE_IN_BYTES	sizeof(void *)
#define ALIGN_MASK			(WORD_SIZE_IN_BYTES-1)

typedef struct _Busy_Header {
	uint32_t size; 	  	  // 31 bits for size and 1 bit for inuse/free; doesn't include header
	unsigned char mem[]; // nothing allocated; just a label to location after size
} Busy_Header;

typedef struct _Free_Header {
	uint32_t size;
	struct _Free_Header *next; // lives inside user data area when free but not when in use
} Free_Header;

/* Pad size n to include header */
inline size_t padsize(size_t n) {
	return n + sizeof(Busy_Header) <= MINSIZE ? MINSIZE : n + sizeof(Busy_Header);
}

/* Align n to nearest word size boundary (4 or 8) */
inline size_t alignsize(size_t n) {
	return (n & ALIGN_MASK) == 0 ? n : (n + WORD_SIZE_IN_BYTES) & ~ALIGN_MASK;
}

int main(int argc, char *argv[])
{
//	printf("sizeof(Busy_Header) == %zu\n", sizeof(Busy_Header));
//	printf("sizeof(Free_Header) == %zu\n", sizeof(Free_Header));
	for (int i=0; i<=MINSIZE; i++) {
		fprintf(stderr, "alignsize(padsize(%d)) == %lu\n", i, alignsize(padsize(i)));
		assert( alignsize(i) == MINSIZE );
	}
	if ( MINSIZE==4 ) {
//		assert(alignsize(5) == 4+3);
	}
	else {
		assert(alignsize(9) == 24);
	}
}