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

#ifndef MALLOC_BINNING_H
#define MALLOC_BINNING_H

#include <stdio.h>
#include <stdlib.h>
#include "morecore.h"

typedef struct _Free_Header {
	uint16_t free; // 1 means free , 0 means in use
	uint16_t size;
	struct _Free_Header *next;
	struct _Free_Header *prev;
} Free_Header;

static const size_t MIN_CHUNK_SIZE = sizeof(Free_Header);

static const size_t WORD_SIZE_IN_BYTES = sizeof(void *);

static const size_t ALIGN_MASK = WORD_SIZE_IN_BYTES - 1;

static const size_t DEFAULT_MAX_HEAP_SIZE = 4096;

static inline size_t size_with_header(size_t n) {
	return n + sizeof(Free_Header) <= MIN_CHUNK_SIZE ? MIN_CHUNK_SIZE : n + sizeof(Free_Header);
}

static inline size_t align_to_word_boundary(size_t n) {
	return (n & ALIGN_MASK) == 0 ? n : (n + WORD_SIZE_IN_BYTES) & ~ALIGN_MASK;
}

static inline size_t request2size(size_t n) {
	return align_to_word_boundary(size_with_header(n));
}

void heap_init();
void *malloc(size_t);
void *calloc(size_t n, size_t size);
void free(void *);
int get_bin_index(size_t);
Free_Header* find_free_buddy(Free_Header*); //merge neighbour free blocks

#endif //MALLOC_BINNING_H
