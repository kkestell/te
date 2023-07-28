#ifndef UTF8_H
#define UTF8_H

#include <cstdint>
#include <string>

namespace utf8
{

// Returns the byte length of a UTF-8 character.
int char_length(char c);

// Returns a substring of a UTF-8 string. The start and len parameters specify UTF-8 character indices, not byte
// indices.
std::string substr(const std::string& str, std::size_t start, std::size_t len);

// Returns the number of UTF-8 characters in a string.
int str_length(const std::string& str);

// Converts a UTF-8 string to a wide character string.
std::wstring to_wide_char(const std::string& str);

// Returns the UTF-8 character index at the given terminal column index. The terminal column index is the position
// on the terminal screen, which may differ from the UTF-8 character index for multi-column characters.
int terminal_to_char_index(const std::string& str, int terminal_index);

// Converts a UTF-8 character to a Unicode code point. Returns the Unicode code point as a string.
std::string char_to_unicode(const std::string& str);

// Converts a Unicode code point to a UTF-8 character. Returns the UTF-8 character as a string.
std::string unicode_to_char(uint32_t c);

} // namespace utf8

#endif
