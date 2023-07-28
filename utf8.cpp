#include "utf8.h"
#include <codecvt>
#include <locale>

static const unsigned char utf8_length_table[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

static const unsigned char utf8_mask[6] = {0x7F, 0x1F, 0x0F, 0x07, 0x03, 0x01};

int utf8::char_length(char c)
{
    return utf8_length_table[(unsigned char)c];
}

std::string utf8::substr(const std::string& str, std::size_t start, std::size_t len)
{
    std::size_t str_len = str.length();
    std::size_t i = 0;
    std::size_t byte_index = 0;
    std::size_t start_byte = 0;
    std::size_t end_byte = 0;

    while (byte_index < str_len && i < (start + len))
    {
        std::size_t char_len = char_length(str[byte_index]);
        if (i >= start)
        {
            if (i == start)
            {
                start_byte = byte_index;
            }
            end_byte = byte_index + char_len;
        }

        byte_index += char_len;
        i += 1;
    }

    return str.substr(start_byte, end_byte - start_byte);
}

int utf8::str_length(const std::string& str)
{
    int length = 0;
    for (size_t i = 0; i < str.size(); )
    {
        i += char_length(str[i]);
        length++;
    }
    return length;
}

std::wstring utf8::to_wide_char(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(str);
}

int utf8::terminal_to_char_index(const std::string& str, int terminal_index)
{
    int term_count = 0;
    int char_index = 0;
    for (size_t i = 0; i < str.size() && term_count < terminal_index; )
    {
        auto substr = utf8::substr(str, i, 1);
        if (!substr.empty())
        {
            term_count += wcwidth(utf8::to_wide_char(substr).front());
        }
        i += char_length(str[i]);
        char_index++;
    }
    return (term_count > terminal_index) ? char_index - 1 : char_index;
}

std::string utf8::char_to_unicode(const std::string& str)
{
    if (str.empty())
        return "";

    int i;
    unsigned char len = char_length(str[0]);
    unsigned char mask = utf8_mask[len - 1];
    uint32_t result = (unsigned char)str[0] & mask;
    for (i = 1; i < len; ++i)
    {
        result <<= 6;
        result |= (unsigned char)str[i] & 0x3f;
    }

    return std::to_string(result);
}

std::string utf8::unicode_to_char(uint32_t c)
{
    std::string out;
    int len = 0;
    int first;
    int i;

    if (c < 0x80)
    {
        first = 0;
        len = 1;
    }
    else if (c < 0x800)
    {
        first = 0xc0;
        len = 2;
    }
    else if (c < 0x10000)
    {
        first = 0xe0;
        len = 3;
    }
    else if (c < 0x200000)
    {
        first = 0xf0;
        len = 4;
    }
    else if (c < 0x4000000)
    {
        first = 0xf8;
        len = 5;
    }
    else
    {
        first = 0xfc;
        len = 6;
    }

    out.resize(len);
    for (i = len - 1; i > 0; --i)
    {
        out[i] = (c & 0x3f) | 0x80;
        c >>= 6;
    }
    out[0] = c | first;

    return out;
}
