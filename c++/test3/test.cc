#include <string.h>
#include <stdio.h>

#ifndef WIN32
#define _strnicmp strncasecmp
#endif

int main(int argc, char *argv[]) {
	if (argc < 3){
		printf("usage: test name1 name2\n");
		return 0;
	}
	if(_strnicmp(argv[1], argv[2], strlen(argv[2]))){
		printf("not equal!\n");
	}else{
        printf("equal!\n");
    }
	return 0;
}
