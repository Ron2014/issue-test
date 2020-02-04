#include "part.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _INIT_ONCE
#define DECL_CTX static
#define RECYCLE_CTX(x)
#else
#define DECL_CTX
#define RECYCLE_CTX(x) free((x))
#endif


EVP_CIPHER_CTX *nalloc() {
    const char *file = __FILE__;
    int line = __LINE__;

    printf("nalloc %s:%d\n", file, line);

    EVP_CIPHER_CTX *ret = (EVP_CIPHER_CTX *)malloc(sizeof(EVP_CIPHER_CTX));
    return ret;
}

void foo() {
    DECL_CTX EVP_CIPHER_CTX *ctx = NULL;
    
    if (!ctx && !(ctx = nalloc())) {
        printf("alloc error!\n");
        abort();
    }

    printf("do foo once!\n");
    RECYCLE_CTX(ctx);
}