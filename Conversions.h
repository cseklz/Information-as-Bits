#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>

class Conversions {
public:
	static std::string toDecimal(const std::string& s); // 0 to 255
	// static std::string toBinary(std::string s); // 00000000 to 11111111
	// static std::string toOctal(std::string s); // 000 to 377
	// static std::string toHexadecimal(std::string s); // 000000 to FFFFFF
};

#endif
