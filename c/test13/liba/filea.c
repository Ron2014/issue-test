#include <stdio.h>
#include "filea.h"
#include "fileb.h"

void fooa() {
    printf("fooa\n");
    FOO_B();
}

inline void _FOO_A() {
    printf("_FOO_A\n");
}