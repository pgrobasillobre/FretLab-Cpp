#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>

class Output {
public:
    Output();

    void out_file_fill(const std::string& in_file);

    std::string output_filename;

};

#endif

