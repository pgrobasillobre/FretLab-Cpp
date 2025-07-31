#ifndef STRING_MANIPULATION_HPP
#define STRING_MANIPULATION_HPP

#include <string>     
#include <iostream>   
#include <stdexcept>  

/// @class String manipulation
/// debugpgi
class String_manipulation {
public:
    void initialize();

    void string_to_float(const std::string& str, double& out); 
};

#endif 
