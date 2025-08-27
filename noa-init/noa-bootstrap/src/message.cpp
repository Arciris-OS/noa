#include <iostream>
#include <ostream>
#include "message.hpp"


void Messages::info(std::string message) {
	std::cout << "\x1b[36;1m=> \x1b[0m" << message << std::endl;
}

void Messages::warn(std::string message) {
	std::cout << "\x1b[33;1m* \x1b[0m" << message << std::endl;
}

void Messages::error(std::string message) {
	std::cout << "\x1b[31;1m### \x1b[0m" << message;
}

