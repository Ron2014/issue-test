#include <iostream>
#include <functional>
using namespace std;

void test0() {
	auto basicLambda = [] { cout << "Hello World" << endl; };
	basicLambda();
}

void test1() {
	auto add = [](int a, int b) -> int { return a + b;};
	auto multiply = [](int a, int b) { return a * b; };

	int sum = add(2,5);
	int product = multiply(2,5);

	cout << sum << endl << product << endl;
}

void test2() {
	int x = 10;

	auto add_x = [x](int a) { return a + x; };
	auto multiply_x = [&x](int a) { return a * x; };

	cout << add_x(10) << " " << multiply_x(10) << endl;
}

void test3() {
	int x = 10;
	auto add_x = [x](int a) mutable { x *= 2; return a + x; };
	cout << add_x(10) << endl;
}

class Filter
{
	public:
		Filter(int divisorVal):
			divisor{divisorVal}
		{}

		std::function<bool(int)> getFilter() 
		{
			return [=](int value) {return value % divisor == 0; };
		}

	private:
		int divisor;
};

int main(int argc, char *argv[]) {
	test3();
}
