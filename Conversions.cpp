#include "Conversions.h"

#include <algorithm>

namespace {
	void addSeparator(std::string& s) {
		if (!s.empty()) {
			s += ' ';
		}
	}

	char toHexDigit(const unsigned int amount) {
		if (amount <= 9) {
			return static_cast<char>('0' + amount);
		}
		return static_cast<char>('A' + (amount - 10));
	}
}

Conversions::Results Conversions::convertAll(const std::string& s) {
	Results result;

	result.decimal = toDecimal(s);
	result.binary = toBinary(s);
	result.octal = toOctal(s);
	result.hexadecimal = toHexadecimal(s);

	return result;
}

std::string Conversions::toDecimal(const std::string& s) {
	std::string result;

	for (const unsigned char c : s) {
		addSeparator(result);

		result += std::to_string(static_cast<unsigned int>(c));
	}

	return result;
}

std::string Conversions::toBinary(const std::string& s) {
	std::string result;

	for (const unsigned char c : s) {
		addSeparator(result);
		for (int bit = 7; bit >= 0; bit--) {
			const unsigned int curr = (c >> bit) & 1U;
			result += curr == 1 ? '1' : '0';
		}
	}

	return result;
}

std::string Conversions::toOctal(const std::string& s) {
	std::string result;

	for (const unsigned char c : s) {
		addSeparator(result);

		unsigned int curr{c};
		std::string octal;

		do {
			octal += static_cast<char>('0' + curr % 8);
			curr /= 8;
		} while (curr > 0);

		std::ranges::reverse(octal);
		result += octal;
	}

	return result;
}

std::string Conversions::toHexadecimal(const std::string& s) {
	std::string result;

	for (const unsigned char c : s) {
		addSeparator(result);

		result += toHexDigit(c / 16);
		result += toHexDigit(c % 16);
	}

	return result;
}