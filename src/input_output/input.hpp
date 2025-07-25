#ifndef INPUT_HPP
#define INPUT_HPP

#include "output.hpp"
#include <string>

class Input {
public:
    Input();

    void get_arguments(int argc, char* argv[]);
//    void check_input_file() const;
//    void read();
//    void print_input_info() const;

    Output out;
    
    std::string input_filename;

private:
    void parse_arguments(int argc, char* argv[]);
};

#endif

