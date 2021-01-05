#include <iostream>
#include "schedule.h"
void func1() {
	std::cout << "111" << std::endl;
	co_start(
		[]
	{
		std::cout << "222" << std::endl;
		co_yield();
		std::cout << "222222" << std::endl;
	}
	);
	std::cout << "111111" << std::endl;
}
void func2() {
	std::cout << "333" << std::endl;
	co_start(
		[]
	{
		std::cout << "444" << std::endl;
		co_yield();
		std::cout << "444444" << std::endl;
	}
	);
	std::cout << "333333" << std::endl;
}

int main()
{
	co_resume(func1);
	co_resume(func2);
	std::cout << "end" << std::endl;
}