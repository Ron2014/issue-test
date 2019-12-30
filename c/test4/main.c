#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef TEST_SHELL
#include <sys/time.h>   // for gettimeofday
#endif

int main(int argc, char *argv[]){
// for time check
#ifdef TEST_TIME
    clock_t t1, t2;
    t1 = clock();
#endif

	if (argc < 2) {
		printf("usage: rand name0 name1 name2 ...\n");
		return 0;
    }

#ifdef TEST_SHELL
    struct timeval t3;
    gettimeofday( &t3, NULL );
    srand((unsigned int)t3.tv_usec);
#else
	srand((unsigned int)time(NULL));
#endif

	int count = argc - 1;

// for 100W test
#ifdef TEST_100W
	for(int i = 0; i < 1000000; i++) {
#endif

	printf("result: %s\n", argv[(rand() % count) + 1]);

#ifdef TEST_100W
	}
#endif

#ifdef TEST_TIME
    t2 = clock();
    printf("%.10f\n", (double)(t2-t1)/CLOCKS_PER_SEC);
#endif

	return 0;
}
