#include <stdio.h>
#include "fileb.h"
#include "filea.h"

void foob() {
    printf("foob\n");
    FOO_A();
}

inline void _FOO_B() {
    printf("_FOO_B\n");
}