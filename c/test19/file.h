#define USE_INLINE
#ifdef USE_INLINE
#define INLINE inline
#else
#define INLINE
#endif

#define FOO() foo()
void foo();
INLINE void foo() {
    printf("foo\n");
}