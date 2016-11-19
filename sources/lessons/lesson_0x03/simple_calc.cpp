#include <iostream>
 
int main() 
{
	double a = 0.0;
	double b = 0.0;
	
	char ch = '\0';
 
	std::cin >> a >> b >> ch;
	
 	if (b != 0)
		std::cout << a / b << std::endl;
	else
		std::cout << "Division by zero!" << std::endl;
	
	return 0;
}