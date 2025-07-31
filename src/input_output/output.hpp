#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "density.hpp"

#include <optional>
#include <string>
#include<fstream>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <ostream>

/// @brief Handles output file naming and writing.
/// 
/// This class is responsible for managing output operations.

class Input; // Forward declaration of Input to avoid circular dependency

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

    /// debugpgi
    void print_density(
        const std::string& filepath,
        const Density& cube,
        std::optional<std::string> header = std::nullopt 
    );

    /// @brief Horizontal line (80 dashes) separation output sections
    const std::string sticks = std::string(80, '-'); 

    /// @brief Gets the internal output stream.
    /// @return Reference to the ofstream object for writing.
    std::ofstream& stream() const;
  
    /// Full path or name of the output file.
    std::string output_filename;



private:

    /// @brief Prints a formatted line to the output stream.
    /// @param out The output stream to write to.
    /// @param i The first integer to format.
    /// @param a The first double to format.
    /// @param b The second double to format.
    /// @param c The third double to format.
    void print_formatted_line1(std::ostream& out, int i, double a, double b, double c);

    /// @brief Prints a formatted line with atom information.
    /// @param out The output stream to write to.
    /// @param atom The atom label (e.g., "H", "O").
    /// @param x The x-coordinate of the atom.
    /// @param y The y-coordinate of the atom.
    /// @param z The z-coordinate of the atom.
    void print_formatted_line2(std::ostream& out, const std::string atom, double x, double y, double z);

    // Define formats for output
    std::string format1 = "   {:5d} {:15.7E} {:15.7E} {:15.7E}\n";

    std::ofstream log_stream; ///< The output file stream

};

#endif 

