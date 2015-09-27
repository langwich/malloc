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
#include "binning.h"
#include "cunit.h"


Heap_Info verify_heap() {
    Heap_Info info = get_heap_info();
    assert_equal(info.heap_size, DEFAULT_MAX_HEAP_SIZE);
    assert_equal(info.heap_size, info.busy_size+info.free_size);
    return info;
}

static void setup()		{ heap_init(); }
static void teardown()	{ verify_heap(); heap_shutdown(); }

void malloc0() {
    void *p = malloc(0);
    assert_addr_not_equal(p, NULL);
    assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void malloc_word_size() {
    void *p = malloc(sizeof(void *));
    assert_addr_not_equal(p, NULL);
    assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void two_malloc_with_free() {
    void *p = malloc(100); // should split heap into two chunks
    assert_addr_not_equal(p, NULL);
    Free_Header *freelist = get_heap_freelist();
    Busy_Header *heap = get_heap_base();
    assert_addr_not_equal(freelist, heap);
    // check 1st chunk
    assert_equal(p, heap);
    assert_equal(chunksize(p), request2size(100));
    // check 2nd chunk
    assert_equal(freelist->size, DEFAULT_MAX_HEAP_SIZE-request2size(100));
    assert_addr_equal(freelist->next, NULL);

    Free_Header *freelist1 = get_bin_freelist(request2size(100)-1);// freelist of bin should be NULL
    free(p);
    Free_Header *freelist2 = get_bin_freelist(request2size(100)-1);//// freelist of bin should be have one element p
    assert_addr_not_equal(freelist1,freelist2);
    void *p1 = malloc(100); // this malloc should be from bin
    assert_addr_not_equal(p1, NULL);
    Free_Header *freelist3 = get_bin_freelist(request2size(100)-1); // freelist of bin should be NULL
    assert_addr_equal(freelist3,NULL);
}

void malloc_large_size_then_free() {
    void *p = malloc(2048);
    assert_addr_not_equal(p, NULL);
    assert_equal(chunksize(p), request2size(2048));
    Free_Header *freelist = get_heap_freelist();
    Busy_Header *heap = get_heap_base();
    assert_addr_not_equal(heap,freelist);
    free(p);
    Busy_Header *heap1 = get_heap_base();
    Free_Header *freelist1 = get_heap_freelist();
    assert_addr_equal(heap1,freelist1);
}

void free_NULL() {
    free(NULL); // don't crash
}

int main(int argc, char *argv[]) {
    cunit_setup = setup;
    cunit_teardown = teardown;

    test(malloc0);
    test(free_NULL);
    test(malloc_word_size);
    test(malloc_large_size_then_free);
    test(two_malloc_with_free);
}

