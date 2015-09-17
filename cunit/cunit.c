#include <stdio.h>
#include "cunit.h"

void (*cunit_setup)()		= NULL;
void (*cunit_teardown)()	= NULL;

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

void cunit_test(void (*f)(), const char funcname[]) {
	if ( cunit_setup!=NULL ) (*cunit_setup)();
	if ( setjmp(longjmp_env)==0 ) {
		f();
		fprintf(stderr, "PASS %s\n", funcname);
	}
	else {
		printf("FAIL %s\n", funcname);
	}
	if ( cunit_teardown!=NULL ) (*cunit_teardown)();
}
