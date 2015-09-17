#include <stdio.h>
#include <stdlib.h>
#include "freelist.h"
#include "cunit.h"

extern void freelist_init(uint32_t max_heap_size);
extern void freelist_shutdown();

const size_t HEAP_SIZE = 2000;

void malloc0() {
	void *p = malloc(0);
	assert_addr_not_equal(p, NULL);
	assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void malloc1() {
	void *p = malloc(1);
	assert_addr_not_equal(p, NULL);
	assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void malloc_word_size() {
	void *p = malloc(sizeof(void *));
	assert_addr_not_equal(p, NULL);
	assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void malloc_2x_word_size() {
	void *p = malloc(2 * sizeof(void *));
	assert_addr_not_equal(p, NULL);
	assert_equal(chunksize(p), 3 * sizeof(void *)); // 2 words + rounded up size field
}

void one_malloc() {
	void *p = malloc(100); // should split heap into two chunks
	assert_addr_not_equal(p, NULL);
	Free_Header *freelist = get_freelist();
	Busy_Header *heap = get_heap_base();
	assert_addr_not_equal(freelist, heap);
	// check 1st chunk
	assert_equal(p, heap);
	assert_equal(chunksize(p), request2size(100));
	// check 2nd chunk
	assert_equal(freelist->size, HEAP_SIZE-request2size(100));
	assert_addr_equal(freelist->next, NULL);
}

void two_malloc() {
	one_malloc(); // split heap into two chunks and test for sanity.
	void *p0 = get_heap_base(); // should be first alloc chunk
	Free_Header *freelist0 = get_freelist();
	Busy_Header *p = malloc(100); // now split sole free chunk into two chunks
	assert_addr_not_equal(p, NULL);
	// check 2nd alloc chunk
	assert_equal(p, freelist0); // should return previous free chunk
	assert_equal(chunksize(p), request2size(100));
	// check remaining free chunk
	Free_Header *freelist1 = get_freelist();
	assert_addr_not_equal(freelist0, freelist1);
	assert_addr_not_equal(freelist0, get_heap_base());
	assert_equal(chunksize(freelist1), HEAP_SIZE-request2size(100)-request2size(100));
	assert_equal(chunksize(p0)+chunksize(p)+chunksize(freelist1), HEAP_SIZE);
	assert_addr_equal(freelist1->next, NULL);
}

void test_core() {
	void *heap = morecore(HEAP_SIZE);
	assert_addr_not_equal(heap, NULL);
	dropcore(heap, HEAP_SIZE);
}

void test_init_shutdown() {
	freelist_init(HEAP_SIZE);
	assert_addr_equal(get_freelist(), get_heap_base());
	freelist_shutdown();
}

static void setup()	{ freelist_init(HEAP_SIZE); }
static void teardown() { freelist_shutdown(); }

int main(int argc, char *argv[]) {
//	long pagesize = sysconf(_SC_PAGE_SIZE); // 4096 on my mac laptop
//	printf("pagesize == %ld\n", pagesize);

	cunit_setup = setup;
	cunit_teardown = teardown;

	test_core();
	test_init_shutdown();

	freelist_init(HEAP_SIZE);

	test(malloc0);
	test(malloc1);
	test(malloc_word_size);
	test(malloc_2x_word_size);
	test(one_malloc);
	test(two_malloc);
}