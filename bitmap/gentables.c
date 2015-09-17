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

/*
 * Use this function to generate 256-way lookup table for
 * n consecutive 0s in 8bit. Implementing first fit policy,
 * and always return the lowest index when we found any
 * suitable suitable space. For those numbers doesn't contain
 * consecutive 0s, the returned value will be -1.
 *
 * Users need to supply a memory chunk with enough empty space.
 *
 * e.g. for 10010001,01101001 and if we require 1 consecutive bit,
 * the table always return 1.
 */

#include <stdio.h>

#define ROW_NUM 256
#define COL_NUM 8

// the initial masks with n leading 0s (from left).
static unsigned char initial_masks[8] = {127, 63, 31, 15, 7, 3, 1, 0};

/*
 * Swap n bits in b start from i, with n bits starts from j.
 * Indices count from right and start from 0. No overlapping!
 *
 * From: http://graphics.stanford.edu/~seander/bithacks.html#SwappingBitsXOR
 */
int swap_bits(int b, int i, int j, int n) {
	int r;
	unsigned int x = ((b >> i) ^ (b >> j)) & ((1U << n) - 1); // XOR temporary
	r = b ^ ((x << i) | (x << j));
	return r;
}

void print_binary(unsigned char b) {
	int i = b;
	for (i = 1 << 7; i > 0; i = i / 2)
		(b & i)? printf("1"): printf("0");
}

/*
 * Expect to have a 256 x 8 2d array.
 * This utility function is meant to generate the 256-way lookup table.
 */
void gen_ff_bitmap(int bit_map[ROW_NUM][COL_NUM]) {
	for (int i = 0; i < ROW_NUM; ++i) {
		for (int j = 0; j < COL_NUM; ++j) {
			unsigned char mask = initial_masks[j];
			for (int k = 0; k < COL_NUM - j; ++k) {
				// if the or-ed result with mask is the same as mask
				// then j consecutive 0s exist at index k in this byte.
				if (((unsigned char)i | mask) == mask) {
					bit_map[i][j] = k;
					break;
				}
				mask = (unsigned char) swap_bits(mask, COL_NUM - 2 - j - k, COL_NUM - 1 - k, 1);
			}
		}
	}
}

void print2d(int arr[ROW_NUM][COL_NUM], int row, int col) {
	printf("{\n");
	for (int i = 0; i < row; ++i) {
		printf("/* %3d */{", i);
		for (int j = 0; j < col; ++j) {
			printf("%2d", arr[i][j]);
			if (j != col - 1) printf(",");
		}
		printf(" }");
		if (i != row - 1) printf(",");
		printf("\n");
	}
	printf("}\n");
}

int main(int argc, char *argv[]) {
	int ff_bitmap[ROW_NUM][COL_NUM] = {{0}};
	for (int i = 0; i < ROW_NUM; ++i) {
		for (int j = 0; j < COL_NUM; ++j) {
			ff_bitmap[i][j] = -1;
		}
	}
	gen_ff_bitmap(ff_bitmap);
	print2d(ff_bitmap, ROW_NUM, COL_NUM);
    return 0;
}