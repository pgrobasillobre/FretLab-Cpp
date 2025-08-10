#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "density.hpp"
#include "integrals.hpp"
#include "nanoparticle.hpp"

#include <optional>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <ostream>

/// @class Output
/// @brief Handles output file naming and writing.
///
/// This class is responsible for managing output operations.

class Input; // Forward declaration of Input to avoid circular dependency

class Output
{
public:
    /// Constructor
    Output();

    /// @brief Creates the output filename based on the input file name.
    void out_file_fill(const std::string &in_file);

    /// @brief Opens the output file for writing.
    ///
    /// It throws if the file cannot be opened.
    void open();

    /// @brief Closes the output file stream.
    void close();

    /// @brief Prints FretLab banner.
    void print_banner();

    /// @brief Fills the output file with density information.
    void print_density(
        const std::string &filepath,
        const Density &cube,
        std::optional<std::string> header = std::nullopt);

    /// @brief Prints nanoparticle information to the output file.
    void print_nanoparticle(const Nanoparticle &np);

    /// @brief Prints integrals' results
    void print_results_integrals(const Target &target, const Integrals &integrals);

    /// @brief Horizontal line (80 dashes) separation output sections
    const std::string sticks = std::string(80, '-');

    /// @brief Gets the internal output stream.
    /// @return Reference to the ofstream object for writing.
    std::ofstream &stream() const;

    /// Full path or name of the output file.
    std::string output_filename;

private:
    /// @brief Prints a formatted line with cube information to the output stream.
    void print_formatted_line1(std::ostream &out, int i, double a, double b, double c);

    /// @brief Prints a formatted line with atom information to the output stream.
    void print_formatted_line2(std::ostream &out, const std::string atom, double x, double y, double z);

    /// @brief Prints a formatted line with nanoparticle information to the output stream.
    void print_formatted_line3(std::ostream &out, const std::string atom, double x, double y, double z);

    // Define formats for output
    std::string format1 = "   {:5d} {:15.7E} {:15.7E} {:15.7E}\n";

    std::ofstream log_stream; ///< The output file stream
};

#endif
