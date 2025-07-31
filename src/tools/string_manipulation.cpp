#include "string_manipulation.hpp"

#include <string>     
#include <iostream>   
#include <stdexcept>  

//debugpgi
//----------------------------------------------------------------------
void String_manipulation::string_to_float(const std::string& str, double& out) {
    try {
        out = std::stod(str);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: '" << str << "' is not a valid float.\n";
    } catch (const std::out_of_range&) {
        std::cerr << "Error: '" << str << "' is out of range for a float.\n";
    }
}
//----------------------------------------------------------------------
