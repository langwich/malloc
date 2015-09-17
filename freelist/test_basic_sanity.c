#include <stdio.h>
#include <stdlib.h>
#include "freelist.h"
#include "../cunit.h"

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
	assert_equal(chunksize(p), MIN_CHUNK_SIZE);
}

void one_malloc() {
	void *p = malloc(1000); // should split heap into two chunks
	assert_addr_not_equal(p, NULL);
	Free_Header *freelist = get_freelist();
	Busy_Header *heap = get_heap_base();
	assert_addr_not_equal(freelist, heap);
	// check 1st chunk
	assert_equal(p, heap);
	assert_equal(chunksize(p), request2size(1000));
	// check 2nd chunk
	assert_equal(freelist->size, HEAP_SIZE-request2size(1000));
	assert_addr_equal(freelist->next, NULL);
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
}