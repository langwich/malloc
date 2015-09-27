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
	assert_equal(bs.m_bc[1], 0x0);
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

void test_bs_set1_right_boundary_hi() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 63);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFFFF);
	assert_equal(bs.m_bc[1], 0x0);
}

void test_bs_set1_right_boundary_lo() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 63, 77);
	assert_equal(bs.m_bc[0], 0xC000000000000001);
	assert_equal(bs.m_bc[1], 0xFFFC000000000000);
}

void test_bs_set1_same_chk() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	assert_equal(bs.m_bc[0], 0xC000000000000000);
	bs_set1(&bs, 2, 3);
	assert_equal(bs.m_bc[0], 0xF000000000000000);
}

void test_bs_set1_same_chk_middle() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	assert_equal(bs.m_bc[0], 0xC000000000000000);
	bs_set1(&bs, 23, 33);
	assert_equal(bs.m_bc[0], 0xC00001FFC0000000);
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

void test_bs_set0_right_boundary_hi() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	bs_set0(&bs, 44, 63);
	assert_equal(bs.m_bc[0], 0xC00001FFFFF00000);
	assert_equal(bs.m_bc[1], 0xFFFF800000000000);
}

void test_bs_set0_right_boundary_lo() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 80);
	bs_set0(&bs, 63, 77);
	assert_equal(bs.m_bc[0], 0xC00001FFFFFFFFFE);
	assert_equal(bs.m_bc[1], 0x0003800000000000);
}

void test_bs_set0_same_chk() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	bs_set1(&bs, 23, 63);//  0xC00001FFFFFFFFFF
	bs_set0(&bs, 24, 33);
	assert_equal(bs.m_bc[0], 0xC00001003FFFFFFF);
	assert_equal(bs.m_bc[1], 0x0);
}

void test_bs_chk_scann() {
	BITCHUNK bchk = 0xFFFFF1FFFFFFF011;
	int index7 = bs_chk_scann(bchk, 7);
	int index4 = bs_chk_scann(bchk, 4);
	int index3 = bs_chk_scann(bchk, 3);
	int index10 = bs_chk_scann(bchk, 10);
	assert_equal(52, index7);
	assert_equal(52, index4);
	assert_equal(20, index3);
	assert_equal(-1, index10);
}

void test_bs_chk_scann_left_bdry() {
	BITCHUNK bchk = 0x0100000000000000;
	int index7 = bs_chk_scann(bchk, 7);
	assert_equal(0, index7);
}

void test_bs_chk_scann_right_bdry() {
	BITCHUNK bchk = 0xFFFFFFFFFFFFFF80;
	int index7 = bs_chk_scann(bchk, 7);
	assert_equal(57, index7);
}

void test_bs_nrun() {
	bitset bs;
	bs_init(&bs, 2, g_heap);
	size_t index2 = bs_nrun(&bs, 2);
	assert_equal(2, index2);
	size_t index12 = bs_nrun(&bs, 12);
	assert_equal(4, index12);
}

int main(int argc, char *argv[]) {
	cunit_setup = setup;
	cunit_teardown = teardown;

	test(test_bs_init);
	test(test_bs_set1);
	test(test_bs_set1_left_boundary);
	test(test_bs_set1_right_boundary_hi);
	test(test_bs_set1_right_boundary_lo);
	test(test_bs_set1_same_chk);
	test(test_bs_set1_same_chk_middle);
	test(test_bs_set0);
	test(test_bs_set0_left_boundary);
	test(test_bs_set0_right_boundary_hi);
	test(test_bs_set0_right_boundary_lo);
	test(test_bs_set0_same_chk);

	test(test_bs_chk_scann);
	test(test_bs_chk_scann_left_bdry);
	test(test_bs_chk_scann_right_bdry);

	test(test_bs_nrun);

	return 0;
}