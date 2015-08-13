/*
 * stringTools.h
 *
 *  Created on: Dec 18, 2014
 *      Author: tobias
 */

#ifndef STRINGTOOLS_H_
#define STRINGTOOLS_H_

#include <string>


/**
 * replaces every non-ASCII character in s with an escape sequence containing the hexadecimal representation of it's value.
 *
 * Additionally backslashes are escaped too, since this character is used to initiate an escape sequence.
 *
 * The value of a valid ASCII is [0, 127].
 *
 * replacement map:
 * ----------------
 * "\"       --> "\\"
 * non-ASCII --> "\x{2 digit hex value}"
 * ASCII     --> ASCII
 *
 */
std::string escape_non_ascii(const std::string &s);


/**
 * unescapes escaped (non-)ASCII characters.
 *
 * replacement map:
 * ----------------
 * "\\"               --> "\"
 * "\x{2 hex digits}" --> character
 * ASCII              --> ASCII
 * everything else    --> exception
 *
 * @see escape_non_ascii
 *
 */
std::string unescape_non_ascii(const std::string &s);


#endif /* STRINGTOOLS_H_ */
