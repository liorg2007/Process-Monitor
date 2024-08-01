#include <iostream>

#include "DataDisplay.h"

int main() {
	try {
		DataDisplay d(3);
	}
	catch(std::exception& e) {
		std::cout << e.what();
	}
}
