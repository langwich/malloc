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
#include <string.h>
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

void _assert_str_equal(const char *a, const char *b, size_t l, const char *as, const char *bs, const char *funcname) {
	if ( strncmp(a, b, l) ) {
		fprintf(stderr, "assertion failure in %s: %s == %s (%s == %s)\n", funcname, as, bs, a, b);
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
		fprintf(stderr, "FAIL %s\n", funcname);
	}
	if ( cunit_teardown!=NULL ) (*cunit_teardown)();
}

