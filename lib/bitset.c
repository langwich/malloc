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

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "bitset.h"

static void init_lup();
static void init_masks();

/*
 * Initializes a bitset with n chunks.
 * See the header for the definition of
 * a chunk.
 */
void bs_init(bitset *bs, size_t nchks, void *pheap) {
	// initialize all kinds of masks.
	init_lup();
	init_masks();

	bs->m_nbc = nchks;
	bs->m_bc = pheap;
	// set the first few bits to 1.
	// Those bits count for the space consumed by the bit score board.
	bs_set1(bs, 0, nchks * CHUNK_SIZE / WORD_SIZE - 1);
}
/*
 * Dump the bit chunk to fd. Mainly used for debug/test.
 */
void bs_dump(BITCHUNK bc, int fd) {
	for (BITCHUNK i = 1ULL << (CHUNK_SIZE_IN_BITS - 1); i > 0; i = i / 2) {
		(bc & i)? write(fd, "1", 1): write(fd, "0", 1);
	}
	write(fd, "\n", 1);
}
/*
 * Returns the index of the first n-run of 0s in the bitset.
 * The index is 0-based.
 */
int bs_nrun(bitset *bs, size_t n) {
	return 0;
}
/*
 * Sets the bits in [lo,hi] to 1.
 * lo and hi are bit indices and are *0-BASED*
 */
int bs_set1(bitset *bs, size_t lo, size_t hi) {
	size_t lo_chk =  ROUND_UP(lo) / (CHUNK_SIZE_IN_BITS);
	size_t hi_chk =  ROUND_DOWN(hi) / (CHUNK_SIZE_IN_BITS);
	for (size_t i = lo_chk; i < hi_chk; ++i) {
		bs->m_bc[i] |= BC_ONE;
	}
	bs->m_bc[lo_chk - 1] |= right_masks[lo_chk * CHUNK_SIZE_IN_BITS - lo];
	bs->m_bc[hi_chk] |= left_masks[hi - hi_chk * CHUNK_SIZE_IN_BITS + 1];
	return 0;
}
/*
 * Sets the bits in [lo,hi] to 0.
 * lo and hi are bit indices and are *0-BASED*
 */
int bs_set0(bitset *bs, size_t lo, size_t hi) {
	size_t lo_chk =  ROUND_UP(lo) / (CHUNK_SIZE_IN_BITS);
	size_t hi_chk =  ROUND_DOWN(hi) / (CHUNK_SIZE_IN_BITS);
	for (size_t i = lo_chk; i < hi_chk; ++i) {
		bs->m_bc[i] &= 0;
	}
	bs->m_bc[lo_chk - 1] &= ~right_masks[lo_chk * CHUNK_SIZE_IN_BITS - lo];
	bs->m_bc[hi_chk] &= ~left_masks[hi - hi_chk * CHUNK_SIZE_IN_BITS + 1];
	return 0;
}

static void init_lup() {

	for (int i = 0; i < LUP_ROW; ++i) {
		for (int j = 0; j < LUP_COL; ++j) {
			ff_lup[i][j] = -1;
		}
	}

	for (int i = 0; i < LUP_ROW; ++i) {
		for (int j = 0; j < LUP_COL; ++j) {
			U1 mask = n_leading0[j];
			for (int k = 0; k < BIT_NUM - j; ++k) {
				// stops when we got j+1 consecutive 0s
				// or we reach the end.
				if (((U1)i | mask) == mask) {
					ff_lup[i][j] = k;
					break;
				}
				mask = (U1) ((1 << 7) | (mask >> 1));
			}
		}
	}
}

static void init_masks() {
	right_masks[0] = 0LL;
	for (int i = 1; i < 65; ++i) {
		right_masks[i] = (right_masks[i-1] << 1) | 1LL;
	}

	left_masks[0] = 0LL;
	for (int i = 1; i < 65; ++i) {
		left_masks[i] = (left_masks[i-1] >> 1) | BC_LEFTMOST_MASK;
	}
}