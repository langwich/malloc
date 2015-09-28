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

#include <cunit.h>
#include <string.h>
#include "bitmap.h"

#define HEAP_SIZE           4096

static void *g_pheap;

static void setup() {
	bitmap_init(HEAP_SIZE);
	g_pheap = bitmap_get_heap();
}

static void teardown() {
	bitmap_release();
}

void test_bitmap_init() {
	assert_equal(0xFF00000000000000, *((BITCHUNK *)g_pheap));
}

void test_bitmap_malloc() {
	void *addr10 = malloc(10);
	// check bit board
	assert_equal(0xFFE0000000000000, *((BITCHUNK *)g_pheap));
	// check address
	assert_addr_equal(addr10, g_pheap + 8);
	// check boundary tag
	assert_equal(0xBBEEEEFF, *((U32 *)addr10));
	assert_equal(3, ((U32 *)addr10)[1]);
}

int main(int argc, char *argv[]) {
	cunit_setup = setup;
	cunit_teardown = teardown;

	test(test_bitmap_init);
	test(test_bitmap_malloc);

	return 0;
}