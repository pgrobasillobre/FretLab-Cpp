#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>

/// @brief Handles output file naming and writing.
/// 
/// This class is responsible for managing output operations.

class Output {
public:
    /// Constructor
    Output();

    /// Creates the output filename based on the input file name.
    /// Example: if input is "test.inp", output becomes "test.out"
    void out_file_fill(const std::string& in_file);

    /// Full path or name of the output file
    std::string output_filename;

};

#endif 

