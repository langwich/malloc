/*
The MIT License (MIT)

Copyright (c) 2015 Terence Parr, Hanzhou Shi, Shuai Yuan, Yuanyuan Zhang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>

/* On my mac laptop Intel Core i7, i get with 32-bit size:
sizeof(Busy_Header) == 4
sizeof(Free_Header) == 16. hmm... it word aligns to 64 bits so next is 8 byte boundary

All chunks must start at 4 or 8-byte boundary. I.e., size & ALIGN_MASK should be 0.
*/

#define MINSIZE				sizeof(Free_Header)
#define WORD_SIZE_IN_BYTES	sizeof(void *)
#define ALIGN_MASK			(WORD_SIZE_IN_BYTES-1)

/* Pad size n to include header */
#define padsize(n)			(n+sizeof(Busy_Header) <= MINSIZE ? MINSIZE : \
 							 n+sizeof(Busy_Header))
/* Align n to nearest word size boundary (4 or 8) */
#define alignsize(n)		(n&ALIGN_MASK==0 ? n : (n+WORD_SIZE_IN_BYTES) & ~ALIGN_MASK)

typedef struct _Busy_Header {
	uint32_t size; 	  	 // 31 bits for size and 1 bit for inuse/free; includes header data
	unsigned char mem[]; // nothing allocated; just a label to location after size
} Busy_Header;

typedef struct _Free_Header {
	uint32_t size;
	struct _Free_Header *next; // lives inside user data area when free but not when in use
} Free_Header;

/* Pointer to the first free chunk in heap */
static Free_Header *freelist;

static Free_Header *nextfree(uint32_t size);

void freelist_init(uint32_t max_heap_size)
{
	printf("sizeof(Busy_Header) == %zu\n", sizeof(Busy_Header));
	printf("sizeof(Free_Header) == %zu\n", sizeof(Free_Header));
	freelist = malloc(max_heap_size + sizeof(Busy_Header));
	freelist->size = max_heap_size & 0x7FFFFFFF; // mask off upper bit to say free
	freelist->next = NULL;
}

void *malloc(size_t size)
{
	if ( freelist==NULL ) {
		return NULL; // out of heap
	}
	uint32_t n = (uint32_t)size & 0x7FFFFFFF;
	n = (uint32_t)alignsize(padsize(n));
	Free_Header *chunk = nextfree(n);
	Busy_Header *b = (Busy_Header *)chunk;
	b->size |= 0x80000000; // get busy! turn on inuse bit at top of size field
	return b;
}

/* Free chunk p by adding to head of free list */
void free(void *p)
{
	if ( p==NULL ) return;
	Free_Header *q = (Free_Header *)p;
	q->next = freelist;
	q->size &= 0x7FFFFFFF; // turn off inuse bit
	freelist = q;
}

/** Find first free chunk that fits size else NULL if no chunk big enough.
 *  Split a bigger chunk if no exact fit, setting size of split chunk returned.
 *  size arg must be big enough to hold Free_Header and padded to 4 or 8
 *  bytes depending on word size.
 */
static Free_Header *nextfree(uint32_t size)
{
	Free_Header *p = freelist;
	Free_Header *prev = NULL;
	/* Scan until one of:
	    1. end of free list
	    2. exact size match between chunk and size
	    3. chunk size big enough to split; there is space for size +
	       another Free_Header (MINSIZE) for the new free chunk.
	 */
	while ( p!=NULL && size != p->size && p->size < size+MINSIZE )
	{
		prev = p;
		p = p->next;
	}
	if ( p==NULL ) return p;	// no chunk big enough

	Free_Header *nextchunk;
	if ( p->size==size )		// if exact fit
	{
		nextchunk = p->next;
	}
	else 						// split p into p', q
	{
		Free_Header *q = (Free_Header *)(((char *)p) + size);
		q->size = size;
		q->next = p->next;
		nextchunk = q;
	}

	// add nextchunk to free list
	if ( p==freelist )		// head of free list is our chunk
	{
		freelist = nextchunk;
	}
	else {
		prev->next = nextchunk;
	}
	return p;
}
