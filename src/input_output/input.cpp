#include "input.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

//----------------------------------------------------------------------
/// @brief Constructor: Initializes the default input file name to "input.inp"
Input::Input() : input_filename("input.inp") {}  // default name
//----------------------------------------------------------------------
/// Delegates to private function for parsing command-line arguments.
void Input::get_arguments(int argc, char* argv[]) {
    parse_arguments(argc, argv);
}
//----------------------------------------------------------------------
/// Parses user-supplied command-line arguments and sets the input filename.
/// Handles three cases:
/// 1. One argument (input filename)
/// 2. No arguments (ask user to type filename)
/// 3. Too many arguments (throws error -- TODO)
void Input::parse_arguments(int argc, char* argv[]) {

    // Normal case: one file passed as argument
    if (argc  == 2)
    input_filename = argv[1];

    out.out_file_fill(input_filename); // Create ouput file name


    // Allow user to type the filename
    if (argc == 1) {
        std::cout << "   Type the input filename (e.g. filename.inp): ";
        std::getline(std::cin, input_filename);  // Read filename with spaces allowed

        out.out_file_fill(input_filename);
        return;
    }

    // To be done for parsing -omp option
    if (argc > 2) {
        throw std::runtime_error("Too many arguments.\n ---> Only one input file is allowed per calculation.");
    }


}
//----------------------------------------------------------------------
// void Input::check_input_file() const {
//     std::ifstream file(input_filename);
//     if (!file) {
//         throw std::runtime_error("Input file not found: " + input_filename);
//     }
// }
//----------------------------------------------------------------------
// void Input::read() {
//     std::ifstream file(input_filename);
//     std::string line;
//     std::cout << "🔍 Reading input file:" << std::endl;
//     while (std::getline(file, line)) {
//         std::cout << "  >> " << line << std::endl;
//         // TODO: parse values and store in members
//     }
// }
// 
//----------------------------------------------------------------------
// void Input::print_input_info() const {
//     std::cout << "📋 Parsed input filename: " << input_filename << std::endl;
//     // TODO: print parsed values here
// }
//----------------------------------------------------------------------



