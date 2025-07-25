#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>
#include<fstream>

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

    /// @brief Opens the output file for writing.
    ///
    /// It throws if the file cannot be opened.
    void open();

    /// @brief Closes the output file stream.
    void close();

    /// @brief Prints FretLab banner.
    void print_banner();

    /// @brief Gets the internal output stream.
    /// @return Reference to the ofstream object for writing.
    std::ofstream& stream();

    /// Full path or name of the output file
    std::string output_filename;

private:
    std::ofstream log_stream; ///< The output file stream

};

#endif 

