#include <stdio.h>
#include <string.h>

char * _strstr(char *a, char *b);

int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("usage: test aaaaaa bbb");
        return 0;
    }

    char *r = _strstr(argv[1], argv[2]);
    if (r==NULL)
        printf("not child string\n");
    else
        printf("child string from %ld\n", r - argv[1]);

    return 0;
}

char * _strstr(char *a, char *b) {
    if (a == NULL || b == NULL)
        return NULL;
    
    int len_a = strlen(a);
    int len_b = strlen(b);
    if (len_b > len_a)
        return NULL;

    int j, match;
    for(int i=0; i < len_a; i++){
        match = i;
        if ((len_a - match) < len_b)
            return NULL;
        
        for (j=0; j < len_b; j++) {
            printf("%c %c\n", a[match+j], b[j]);
            if (a[match+j] != b[j]) {
                match = -1;
                break;
            }
        }

        if (match >= 0)
            break;
    }
    
    if (match < 0)
        return NULL;
    
    return a+match;
}
