#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>

class Conversions {
public:
	struct Results {
		std::string decimal;
		std::string binary;
		std::string octal;
		std::string hexadecimal;
	};

	static Results convertAll(const std::string& s);

private:
	static std::string toDecimal(const std::string& s);
	static std::string toBinary(const std::string& s);
	static std::string toOctal(const std::string& s);
	static std::string toHexadecimal(const std::string& s);
};

#endif