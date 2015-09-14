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

typedef struct _Busy_Header {
	u_int32_t size; // 31 bits for size and 1 bit for inuse/free
} Busy_Header;

typedef struct _Free_Header {
	u_int32_t size;
	struct _Free_Header *next;
} Free_Header;

/* Pointer to the first free chunk in heap */
static Free_Header *freelist;

void freelist_init(u_int32_t max_heap_size)
{
	freelist = malloc(max_heap_size + sizeof(u_int32_t));
	freelist->size = max_heap_size & 0x7FFFFFFF; // mask off upper bit to say free
	freelist->next = NULL;
}

void *freelist_malloc(size_t size)
{
	u_int32_t n = (u_int32_t)size;
	// find first chunk that fits size

}

void freelist_free(void *p)
{
}
