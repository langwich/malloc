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

#include "binning.h"
#include <morecore.h>
#include <stddef.h>

void * heap; // each time request a fixed max size memory 4096 byte

/* ten fixed size bin, each bin contains a free list, max size for malloc
* is 4096 bytes, for each malloc request, it will start from bin 0 to find
* the best fit size, return the index of bin,then return address.
* If there is no free block in bin[index], continue search next index(index++),
* till find one (index <=9),for loop to divide the free block by 2 till the
* size best fit the request.at the same time add the rest unused free block to
* each bin against size.
* if index > 9, request new heap, repeat divide free block and add free block
* to free list, and then return address finally.
*/
Free_Header * bin[10];
/**** SIZES FOR EACH BIN ****
 * bin[0] -> 8
 * bin[1] -> 16
 * bin[2] -> 32
 * bin[3] -> 64
 * bin[4] -> 128
 * bin[5] -> 256
 * bin[6] -> 512
 * bin[7] -> 1024
 * bin[8] -> 2048
 * bin[9] -> 4096
 */

void heap_init(){

	heap = morecore(DEFAULT_MAX_HEAP_SIZE);
	bin[9] = (Free_Header *) heap;
	bin[9]->free = 1;
	bin[9]->size = DEFAULT_MAX_HEAP_SIZE;
	bin[9]->next = NULL;
	bin[9]->prev = NULL;
}

void *malloc(size_t size) {
	size_t actual_size = request2size(size);
	if(actual_size > DEFAULT_MAX_HEAP_SIZE) return NULL;
}

void *calloc(size_t n, size_t size){
}

void free(void *ptr) {

}

int get_bin_index(size_t){

}

Free_Header* find_free_buddy(Free_Header*){

}