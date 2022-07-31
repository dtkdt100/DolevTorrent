#include "Exception.h"

Exception::Exception(ExceptionType eType): eType(eType) {
}


void Exception::printException() {
	std::cout << "Exception: " << eType << std::endl;
}