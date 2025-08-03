#ifndef INPUT_HPP
#define INPUT_HPP

#include "output.hpp"
#include "string_manipulation.hpp"
#include "target.hpp"

#include <string>


///
/// @class Input
/// @brief Input module for handling user input and arguments.
///
class Input {
public:
    /// Constructor: sets default input file name ("input.inp")
    Input();

    /// Parses command-line arguments to set input file name
    void get_arguments(int argc, char* argv[], Output& out);

    /// @brief Check input file existence and extension.
    /// @param Output stream to write the report to (default is std::cout).
    void check_input_file(const Output& out);

    /// @brief Prints input file information to the output stream.
    void print_input_info(const Output& out, const Target& target);

    /// @brief Reads the input file.
    void read(Target& target); 

    /// @brief Determine the target calculation based on input
    void get_target(Target& target);

    // Density integration
    std::string input_filename;
    std::string density_file_integration; ///< File for density integration (full path)
    std::string density_file_integration_input; /// File for density integration as named in input

    // Acceptor 
    bool is_acceptor_density_present = false;

    std::string acceptor_density_file; ///< File for acceptor density (full path)
    std::string acceptor_density_input_file; /// File for acceptor density as named in input

    // Donor
    bool is_donor_density_present = false;

    std::string donor_density_file; ///< File for donor density (full path)
    std::string donor_density_input_file; /// File for donoe density as named in input

    // Target + other options
    TargetMode target_mode = TargetMode::None;  ///< Selected calculation target

    bool is_cutoff_present = false;
    double cutoff = 0.0; 

    bool is_spectral_overlap_present = false;
    double spectral_overlap = 0.0;

    Output out;
    Target target;
    String_manipulation str_manipulation;

private:

    /// @brief Parses command-line arguments and sets the input filename.
    /// @param argc Argument count from main()
    /// @param argv Argument vector from main()
    /// @param out Output object for logging or error reporting
    void parse_arguments(int argc, char* argv[], Output& out);

    ///
    /// @brief Checks if the specified file exists.
    ///
    /// Throws a runtime error if the file is not accessible.
    ///
    /// @param path Absolute or relative path to the file to check
    void file_exists(const std::string& path) const;

    /// @brief Resolves a relative file path to an absolute one based on the input file's location.
    ///
    /// This allows paths given in the input file to be relative to the input file's directory.
    ///
    /// @param relative_path Path specified in the input file (usually relative)
    /// @return The absolute, normalized path as a string
    std::string resolve_relative_to_input(const std::string& relative_path) const;

    /// @brief Combines resolving, existence check, and storage of input file paths.
    ///
    /// This helper function:
    /// - Stores the raw input string (as given by the user)
    /// - Resolves its full absolute path
    /// - Verifies that the file exists
    /// - Stores the resolved path
    ///
    /// @param raw_input The filename/path string as read from the input file
    /// @param input_field Reference to the variable where raw_input should be stored
    /// @param resolved_field Reference to the variable where the resolved full path should be stored
    void check_and_store_file(const std::string& raw_input,
                              std::string& input_field,
                              std::string& resolved_field) const;
                              
};

#endif

