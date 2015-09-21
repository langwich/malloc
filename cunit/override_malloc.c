#include <stdio.h>
#include <stdlib.h>

void *parrt_malloc(size_t n) {
	void *p = malloc(n);
	printf("malloc %zu -> %p\n", n, p);
	//printf("%zu\n", n);
	return p;
}

void parrt_free(void *p) {
	printf("free %p\n", p);
	free(p);
}
