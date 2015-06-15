/*
 * stringTools.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: tobias
 */

#include <climits>
#include <string>    // std::string
#include <stdexcept> // std::invalid_argument


std::string escape_non_ascii(const std::string &s) {
	std::string result;
	for(const auto c : s) {
		// MSB is set --> not a valid ASCII character --> escape
		if (c & (1 << (CHAR_BIT - 1))) {
			static const char hex_map [] = "0123456789abcdef";
			const unsigned char uc = static_cast<unsigned char>(c);
			result += "\\x";
			result += hex_map[uc >> 4];
			result += hex_map[uc & 0xf];
		}
		// escape backslashes since this character initiates escape sequences
		else if (c == '\\') {
			result += "\\\\";
		}
		else {
			result += c;
		}
	}
	return result;
}


std::string unescape_non_ascii(const std::string &s) {

	const auto advance = [&s] (std::string::const_iterator &it) {
		if (++it == s.cend()) {
			throw std::invalid_argument("unexpected end of string");
		}
	};

	const auto from_hex = [] (char c) -> unsigned char {
		if ('0' <= c && c <= '9') {
			return static_cast<unsigned char>(c - '0');
		}
		if ('a' <= c && c <= 'f') {
			return static_cast<unsigned char>(c - 'a' + 10);
		}
		throw std::invalid_argument("unexpected non-hex character in escape sequence");
	};

	std::string result;
	for(auto it = s.cbegin(); it != s.cend(); ++it) {
		if (*it & (1 << (CHAR_BIT - 1))) {
			throw std::invalid_argument("string contains a non-ASCII character");
		}
		// begin of escape sequence
		if (*it == '\\') {
			advance(it);
			// this is an escaped backslash
			if (*it == '\\') {
				result += '\\';
			}
			// the following two chars encode one char
			else if (*it == 'x') {
				advance(it);
				unsigned char c = from_hex(*it);
				advance(it);
				c = static_cast<char>(c << 4 | from_hex(*it));
				result += c;
			}
			else {
				throw std::invalid_argument("unexpected character in escape sequence");
			}
		}
		// regular ASCII char
		else {
			result += *it;
		}
	}
	return result;
}
