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
#include <morecore.h>

#include "bitmap.h"

static void *g_pheap;
static size_t g_heap_size;
static bitset g_bset;

/*
 * Current implementation is really straightforward. We don't
 * dynamically adjust the arena size.
 */
void bitmap_init(size_t size) {
	g_pheap = morecore(size);
	g_heap_size = size;
	// the bitset will "borrow" some heap space here for the bit "score-board".
	bs_init(&g_bset, size / (CHK_IN_BIT * WORD_SIZE) + 1, g_pheap);
}

void bitmap_release() {
	dropcore(g_pheap, g_heap_size);
}

/*
 * This function is used to return the start address of required
 * amount of heap/mapped memory.
 * The size is round up to the word boundary.
 * NULL is returned when there is not enough memory.
 */
void *malloc(size_t size)
{
	size_t n = ALIGN_WORD_BOUNDARY(size);
	int run_index = bs_nrun(&g_bset, n / WORD_SIZE);
	return g_pheap + run_index;
}

/*
 * This function returns the memory to arena and unmarks the
 * related bits in bitset.
 */
void free(void *ptr)
{

}

#ifdef DEBUG
void *bitmap_get_heap() {
	return g_pheap;
}
#endif
