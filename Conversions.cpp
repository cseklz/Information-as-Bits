#include "Conversions.h"

std::string Conversions::toDecimal(const std::string& s) {
	std::string ret;

	for (const unsigned char c : s) {
		if (!ret.empty()) {
			ret += ' ';
		}

		ret += std::to_string(static_cast<unsigned int>(c));
	}

	return ret;
}

std::string Conversions::toBinary(const std::string& s) {
	std::string ret;

	for (const unsigned char c : s) {
		if (!ret.empty()) {
			ret += ' ';
		}

		std::string tmp;

		for (int bit = 7; bit >= 0; bit--) {
			unsigned int curr = (c >> bit) & 1;
			tmp += curr == 1 ? '1' : '0';
		}

		ret += tmp;
	}

	return ret;
}

std::string Conversions::toOctal(const std::string& s) {
	std::string ret;

	for (const unsigned char c : s) {
		if (!ret.empty()) {
			ret += ' ';
		}

		unsigned int curr { c };
		std::string tmp;

		do {
			tmp += std::to_string(curr % 8);
			curr /= 8;
		} while (curr > 0);

		std::ranges::reverse(tmp);
		ret += tmp;
	}

	return ret;
}