#include <iostream>
 
int main() 
{
	double a = 0.0;
	double b = 0.0;
	
	char ch = '\0';
 
	std::cin >> a >> b >> ch;
	
	if (ch == '+')
		std::cout << "a + b = " << a + b << std::endl;
	else if (ch == '-')
		std::cout << "a - b = " << a - b << std::endl;
	else if (ch == '*')
		std::cout << "a * b = " << a * b << std::endl;
	else if (ch == '/')
	{
		if (b != 0)
			std::cout << "a / b = " << a / b << std::endl;
		else
			std::cout << "Division by zero!" << std::endl;
	}
	else
		std::cout << "Unknown operation '" << ch << "'" << std::endl;
	
	return 0;
}