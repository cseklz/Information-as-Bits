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

// unsigned int toBinary(unsigned char c) {
// 	unsigned int n { Conversions::toDecimal(c) };
// }