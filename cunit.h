#include <setjmp.h>

extern void (*cunit_setup)();
extern void (*cunit_teardown)();

#define test(f)						cunit_test(f, #f)
#define assert_equal(a,b)			_assert_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_not_equal(a,b)		_assert_not_equal((unsigned long)a,(unsigned long)b,#a,#b,__func__)
#define assert_addr_equal(a,b)		_assert_addr_equal(a,b,#a,#b,__func__)
#define assert_addr_not_equal(a,b)	_assert_addr_not_equal(a,b,#a,#b,__func__)

void _assert_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]);
void _assert_not_equal(unsigned long a, unsigned long b, const char as[], const char bs[], const char funcname[]);
void _assert_addr_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]);
void _assert_addr_not_equal(void *a, void *b, const char as[], const char bs[], const char funcname[]);

void cunit_test(void (*f)(), const char funcname[]);
