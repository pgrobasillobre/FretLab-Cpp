#include "input.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

Input::Input() : input_filename("input.inp") {}  // default name

void Input::get_arguments(int argc, char* argv[]) {
    parse_arguments(argc, argv);
}

void Input::parse_arguments(int argc, char* argv[]) {
    if (argc > 1) {
        input_filename = argv[1];
    }
    std::cout << " Input file set to: " << input_filename << std::endl;
}
// 
// void Input::check_input_file() const {
//     std::ifstream file(input_filename);
//     if (!file) {
//         throw std::runtime_error("Input file not found: " + input_filename);
//     }
// }
// 
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
// void Input::print_input_info() const {
//     std::cout << "📋 Parsed input filename: " << input_filename << std::endl;
//     // TODO: print parsed values here
// }

