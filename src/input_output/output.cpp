#include "output.hpp"  
#include <iostream>
#include <fstream>
#include <stdexcept>

/// @brief Constructor for Output.
Output::Output() {} 

//----------------------------------------------------------------------
void Output::out_file_fill(const std::string& in_file) {

    /// @brief Generate the output filename based on the input filename.
    /// 
    /// This function removes the last 4 characters of the input filename
    /// (typically ".inp") and replaces them with ".log". For example:
    /// - input: "test.inp"
    /// - output: "test.log"
    /// 
    /// It also prints the new output filename to the console.
    ///
    /// @param in_file Name of the input file

    output_filename = in_file.substr(0, in_file.size() - 4) + ".log";
    std::cout << "output filename is now " << output_filename << std::endl;
}
//----------------------------------------------------------------------


