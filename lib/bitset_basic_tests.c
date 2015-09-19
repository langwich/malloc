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
#include "bitset.h"

#define HEAP_SIZE           4096

static char g_heap[HEAP_SIZE];

static void setup() {
	memset(g_heap, 0, HEAP_SIZE);
}

static void teardown() { }

void test_bs_init() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	assert_equal(bs.m_bc[0], 0xC000000000000000);
	assert_equal(bs.m_bc[1], 0ULL);
}

void test_bs_set1() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFFFF);
	assert_equal(bs.m_bc[1], 0xFFFF800000000000);
}

void test_bs_set1_left_boundary() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 64, 80);
	assert_equal(bs.m_bc[0], 0xC000000000000000);
	assert_equal(bs.m_bc[1], 0xFFFF800000000000);
}

void test_bs_set1_right_boundary() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 63);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFFFF);
	assert_equal(bs.m_bc[1], 0x0);
}

void test_bs_set0() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	bs_set0(&bs, 55, 77);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFE00);
	assert_equal(bs.m_bc[1], 0x0003800000000000);
}

void test_bs_set0_left_boundary() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	bs_set0(&bs, 64, 77);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFFFF);
	assert_equal(bs.m_bc[1], 0x0003800000000000);
}

void test_bs_set0_right_boundary() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	bs_set0(&bs, 44, 63);
	assert_equal(bs.m_bc[0], 0xC00001FFFFF00000);
	assert_equal(bs.m_bc[1], 0xFFFF800000000000);
}

int main(int argc, char *argv[]) {
	cunit_setup = setup;
	cunit_teardown = teardown;

	test(test_bs_init);
	test(test_bs_set1);
	test(test_bs_set1_left_boundary);
	test(test_bs_set1_right_boundary);
	test(test_bs_set0);
	test(test_bs_set0_left_boundary);
	test(test_bs_set0_right_boundary);

	return 0;
}