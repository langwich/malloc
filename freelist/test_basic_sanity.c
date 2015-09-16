#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "freelist.h"

extern void freelist_init(uint32_t max_heap_size);
extern void freelist_shutdown();

const size_t HEAP_SIZE = 2000;

static void setup() 	{ freelist_init(HEAP_SIZE); }
static void shutdown()	{ freelist_shutdown(); }

// ------ begin test support code ------
#define test(f)						_test(f, #f)
#define assert_equal(a,b)			_assert_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_not_equal(a,b)		_assert_not_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_addr_equal(a,b)		_assert_addr_equal(a,b,#a,#b,__func__)
#define assert_addr_not_equal(a,b)	_assert_addr_not_equal(a,b,#a,#b,__func__)

static jmp_buf longjmp_env;

void _assert_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]) {
	if ( a!=b ) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%lu == %lu)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_not_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]) {
	if ( a==b ) {
		fprintf(stderr, "assertion failure in %s: %s != %s (%lu == %lu)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_addr_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]) {
	if ( a!=b ) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%p == %p)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

void _assert_addr_not_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]) {
	if ( a==b ) {
		fprintf(stderr, "assertion failure in %s: %s != %s (%p == %p)\n", funcname, as, bs, a, b);
		longjmp(longjmp_env, 1);
	}
}

static void _test(void (*f)(), const char funcname[]) {
	setup();
	if ( setjmp(longjmp_env)==0 ) {
		f();
		fprintf(stderr, "PASS %s\n", funcname);
	}
	else {
		printf("FAIL %s\n", funcname);
	}
	shutdown();
}
// ------ end test support code ------

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

int main(int argc, char *argv[]) {
//	long pagesize = sysconf(_SC_PAGE_SIZE); // 4096 on my mac laptop
//	printf("pagesize == %ld\n", pagesize);

	test_core();
	test_init_shutdown();

	freelist_init(HEAP_SIZE);

	test(malloc0);
	test(malloc1);
	test(malloc_word_size);
	test(malloc_2x_word_size);
	test(one_malloc);
}