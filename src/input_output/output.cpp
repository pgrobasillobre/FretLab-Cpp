#include "output.hpp"  
#include <iostream>
#include <iomanip> 
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
}
//----------------------------------------------------------------------
/// @brief Opens the output file for writing.
void Output::open() {
    log_stream.open(output_filename, std::ios::out);
    if (!log_stream.is_open()) {
        throw std::runtime_error("Failed to open output file: " + output_filename);
    }
}
//----------------------------------------------------------------------
/// @brief Gets the internal output stream.
std::ofstream& Output::stream() {
    return log_stream;
}
//----------------------------------------------------------------------
/// @brief Closes the output file stream.
void Output::close() {
    if (log_stream.is_open()) {
        log_stream.close();
    }
}
//----------------------------------------------------------------------
/// @brief Prints FretLab banner
void Output::print_banner() {
    const std::string indent = std::string(20, ' ');

    log_stream << " " << std::string(80, '-') << "\n \n";

    log_stream << indent << "    ______          __  __          __  \n";
    log_stream << indent << "   / ____/_______  / /_/ /   ____ _/ /_ \n";
    log_stream << indent << "  / /_  / ___/ _  / __/ /   / __ `/ __ |\n";
    log_stream << indent << " / __/ / /  /  __/ /_/ /___/ /_/ / /_/ /\n";
    log_stream << indent << "/_/   /_/   |___/ __/_____/__,_/_.___/  \n";
    log_stream << indent << "                                        \n";

    log_stream << " \n " << std::string(80, '-') << "\n \n";
    log_stream << std::string(25, ' ') << "Program by Pablo Grobas Illobre\n\n";
    log_stream << " " << std::string(80, '-') << "\n\n";
    log_stream.flush();
}
//----------------------------------------------------------------------

