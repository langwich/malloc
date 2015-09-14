#include <stdio.h>
#include <stdint.h>

typedef struct _Busy_Header {
	uint32_t size; 	  	  // 31 bits for size and 1 bit for inuse/free; doesn't include header
	uint32_t unused;  // 8-byte alignment
	unsigned char mem[]; // nothing allocated; just a label to location after size
} Busy_Header;

typedef struct _Free_Header {
	uint32_t size;
	struct _Free_Header *next; // lives inside user data area when free but not when in use
} Free_Header;

int main(int argc, char *argv[])
{
	printf("sizeof(Busy_Header) == %zu\n", sizeof(Busy_Header));
	printf("sizeof(Free_Header) == %zu\n", sizeof(Free_Header));
}