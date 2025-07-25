
#ifndef INPUT_HPP
#define INPUT_HPP

#include "output.hpp"
#include <string>

/// @brief Handles input file reading
///
/// Input module for handling user input and arguments.
class Input {
public:
    /// Constructor: sets default input file name ("input.inp")
    Input();

    /// Parses command-line arguments to set input file name
    void get_arguments(int argc, char* argv[]);

//    void check_input_file() const;
//    void read();
//    void print_input_info() const;

    Output out;

    std::string input_filename;

private:
    /// Internal parser for command-line arguments
    void parse_arguments(int argc, char* argv[]);
};

#endif

