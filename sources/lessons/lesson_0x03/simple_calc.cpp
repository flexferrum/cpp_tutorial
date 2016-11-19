#include <iostream>
 
int main() 
{
	double a = 0.0;
	double b = 0.0;
	
	char ch = '\0';
 
	std::cin >> a >> b >> ch;
	
	double result = 0.0;
	switch (ch)
	{
	case '+':
		result = a + b;
		break;
	case '-':
		result = a - b;
		break;
	case '*':
		result = a * b;
		break;
	case '/':
	{
		if (b != 0)
			std::cout << "a / b = " << a / b << std::endl;
		else
			std::cout << "Division by zero!" << std::endl;
		break;
	}
	default:
		std::cout << "Unknown operation '" << ch << "'" << std::endl;
		return 0;
	}
	
	std::cout << "a " << ch << " b = " << result << std::endl;
	
	return 0;
}