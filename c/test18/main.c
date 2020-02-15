#include <stdio.h>
#include <stdlib.h>

int bit_len(unsigned int val){
    int ret = 1;
    while(val>>=1) {
        ret++;
    }
    return ret;
}

void print_bit(int val) {
    int len = sizeof(int)*8;
    printf("%zd ", sizeof(int));

    char *p = (char *)malloc(len+1);
    p[len] = '\0';

    int b = 0;
    while(len--) {
        if (val & (1<<b++))
            p[len] = '1';
        else
            p[len] = '0';
    }

    printf("%s\n", p);
}

int bit_count(unsigned int val) {
    int ret = 0;
    int mask = sizeof(val) * 8;
    while(mask--){
        if (val & (1<<mask)) ret++;
    }
    return ret;
}

int main(int argc, char *argv[]) {
    unsigned int val;

    while(--argc) {
        val = atoi(argv[argc]);
        printf("%d 0x%x\n", val, val);
        print_bit(val);
        printf("bit len %d\n", bit_len(val));
        printf("bit count %d\n", bit_count(val));
        
        printf("\n");
    }

    return 0;
}