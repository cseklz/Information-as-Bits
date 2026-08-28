#ifndef CONVERIONS_H
#define CONVERIONS_H

class Conversions {
public:
	int toDecimal(char c); // 0 to 255
	int toBinary(char c); // 00000000 to 11111111
	int toOctal(char c); // 0 to 377
	int toHexadecimal(char c); // 000000 to FFFFFF
};

#endif
