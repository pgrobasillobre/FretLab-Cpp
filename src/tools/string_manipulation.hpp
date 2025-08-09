#ifndef STRING_MANIPULATION_HPP
#define STRING_MANIPULATION_HPP

#include <string>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string_view>

/// @class String manipulation
/// debugpgi
class String_manipulation
{
public:
    void initialize();

    void string_to_float(const std::string &str, double &out);

    void string_to_int(const std::string &str, int &out);
};


// Returns true if marker was found (stream positioned on the line AFTER it).
// If not found, returns false and the stream will be at EOF.
inline bool go_to_string(std::istream &in, std::string_view marker)
{
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // CRLF-safe
        if (line == marker)
            return true; // positioned after marker line
    }
    return false;
}

#endif
