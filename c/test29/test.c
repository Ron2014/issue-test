<<<<<<< HEAD
#include <stdio.h>
int main() {
	double a = 0.0;
	double b = 0.0;
	double c = 1.0;
	double d = b / a;	// -nan, so is a/a, 'Not a Number'
	double e = c / a;	// inf

	int f = 0;
	int g = 0;
	int h = 1;
//	int i = 0;
	int i = f/f;		// 1, MacOS: Floating point exception: 8
	printf("%.2f %.2f %d\n", d, e, i);

  	//int j = g/f;		// crash
	//int k = h/f;		// crash
	return 0;
}
=======
#include <stdio.h>
int main() {
	double a = 0.0;
	double b = 0.0;
	double c = 1.0;
	double d = b / a;	// -nan, so is a/a, 'Not a Number'
	double e = c / a;	// inf

	int f = 0;
	int g = 0;
	int h = 1;
  	int i = f/f;		// 1
	printf("%.2f %.2f %d\n", d, e, i);

  	//int j = g/f;		// crash
	//int k = h/f;		// crash
	return 0;
}
>>>>>>> 1885d36025f93dd169260c9a55d4b1304d57c8bd
