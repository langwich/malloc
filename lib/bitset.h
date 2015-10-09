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

#ifndef MALLOC_BITSET_H
#define MALLOC_BITSET_H

#include <stddef.h>
#include <stdbool.h>

typedef unsigned long long      BITCHUNK;// one full chunk covers 512 bytes in the heap.
typedef unsigned char           U1;
typedef __uint32_t              U32;

#define WORD(x)                 ((unsigned long long *)x)
#define BITSET_NON              ((BITCHUNK) ~0x0)// we can never get that much memory
#define BIT_NUM                 8
#define WORD_SIZE               (sizeof(void *))
#define ALIGN_MASK              (WORD_SIZE - 1)
#define CHUNK_SIZE              (sizeof(BITCHUNK))// usually it's the same as WORD_SIZE on 64-bit machines.
#define CHK_IN_BIT              (CHUNK_SIZE * BIT_NUM)
#define BC_ONE                  0xFFFFFFFFFFFFFFFF
#define BC_LEFTMOST_MASK        0x8000000000000000

#define ALIGN_WORD_BOUNDARY(n)  ((n & ALIGN_MASK) == 0 ? n : (n + WORD_SIZE) & ~ALIGN_MASK)

/* this struct holds the actual data of
 * the bitset.
 */
typedef struct {
	BITCHUNK *m_bc;
	size_t m_nbc;
} bitset;

typedef struct {
	long leading;
	long trailing;
	long non_cross;
} profile_info;

void bs_init(bitset *, size_t, void *);
size_t bs_nrun(bitset *, size_t);
void bs_set1(bitset *, size_t, size_t);
void bs_set0(bitset *, size_t, size_t);
int bs_chk_scann(BITCHUNK, size_t);
int bs_contain_ones(bitset *, size_t, size_t);
profile_info get_profile_info();

void bs_dump(BITCHUNK, int);

#endif //MALLOC_BITSET_H
