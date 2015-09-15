#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern void freelist_init(uint32_t max_heap_size);

const size_t HEAP_SIZE = 2000;

void test_one_malloc() {
	void *p = malloc(1000);
	assert(p!=NULL);
}

void test_malloc0() {
	void *p = malloc(0);
	assert(p!=NULL);
}

int main(int argc, char *argv[]) {
	freelist_init(HEAP_SIZE);
	test_malloc0();
	test_one_malloc();
}