#include "input.hpp"
#include "string_manipulation.hpp"
#include "target.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <algorithm> // for std::transform
#include <stdexcept>

//----------------------------------------------------------------------
///
/// @brief Constructor: Initializes the default input file name to "input.inp"
///
Input::Input() : input_filename("input.inp") {}  // default name
//----------------------------------------------------------------------
///
/// @brief Delegates to private function for parsing command-line arguments.
///
void Input::get_arguments(int argc, char* argv[], Output & out, Target& target) {
    parse_arguments(argc, argv, out);
    target.input_filename = input_filename; // Assign variable to target class 
}
//----------------------------------------------------------------------
///
/// @brief Parses user-supplied command-line arguments and sets the input filename.
/// Handles three cases:
/// 1. One argument (input filename)
/// 2. No arguments (ask user to type filename)
/// 3. Too many arguments (throws error -- TODO)
///
void Input::parse_arguments(int argc, char* argv[], Output& out) {

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
///
/// @brief Check that files exists and has the supported extension (.inp).
void Input::check_input_file(const Output& out) {
    std::ifstream file(input_filename);
    if (!file) {
        throw std::runtime_error("File " + input_filename + " does not exist. Check the file name and path.");
    }

    // Creates new string variable (lowercase) to check extension
    std::string lower_filename = input_filename;
    std::transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);

    // Check file extension is .inp
    if (lower_filename.size() < 4 || lower_filename.substr(lower_filename.size() - 4) != ".inp") {
        throw std::runtime_error("File " + input_filename + " does not have the supported extension (.inp).");
    }
}
//----------------------------------------------------------------------
///
/// @brief Reads the input file and parses its content.
///
void Input::read(Target& target) {

    // Open the input file.
    std::ifstream file(input_filename);
    file_exists(input_filename); // Check file existence

    // Step 1: Create a dispatch table (a keyword-to-function map)
    std::unordered_map<std::string, std::function<void(const std::string&)>> handlers;

    // Step 2: Define how each keyword should be handled
    // ========
    handlers["integrate cube file"] = [&](const std::string& value) {
        check_and_store_file(value, target.density_file_integration_input, target.density_file_integration);
        target.integrate_density = true;
    };
    // ========
    handlers["acceptor density"] = [&](const std::string& value) {
        check_and_store_file(value, target.acceptor_density_input_file, target.acceptor_density_file);
        target.is_acceptor_density_present = true;
    };
    // ========
    handlers["donor density"] = [&](const std::string& value) {
        check_and_store_file(value, target.donor_density_input_file, target.donor_density_file);
        target.is_donor_density_present = true;
    };
    // ========
    handlers["cutoff"] = [&](const std::string& value) {
        str_manipulation.string_to_float(value, target.cutoff);
        if (target.cutoff < 0.0) throw std::runtime_error("Cutoff cannot be negative.");
        target.is_cutoff_present = true;
    };
    // ========
    handlers["spectral overlap"] = [&](const std::string& value) {
        str_manipulation.string_to_float(value, target.spectral_overlap);
        target.is_spectral_overlap_present = true;
        if (target.spectral_overlap < 0.0) throw std::runtime_error("Spectral overlap cannot be negative.");
    };
    // ========
    handlers["omega_0"] = [&](const std::string& value) {
        str_manipulation.string_to_float(value, target.omega_0);
        target.is_omega_0_present = true;
        if (target.omega_0 < 0.0) throw std::runtime_error("omega_0 cannot be negative");
    };
    // ========







    std::string line;
    // Step 3: Read input file line-by-line
    while (std::getline(file, line)) {
        // Trim leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines or comment lines (starting with '#' or '!')
        if (line.empty() || line[0] == '#' || line[0] == '!') continue;

        // Step 4: Split line into keyword and value using ':'
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) {
            continue;  // no colon found — skip or optionally throw an error
        }

        std::string key = line.substr(0, colon_pos);
        std::string value = line.substr(colon_pos + 1);

        // Trim whitespace from  both key andvalue
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Convert key to lowercase to make it case-insensitive
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        // Step 5: Call the handler function if the key is recognized
        if (handlers.contains(key)) {
            handlers[key](value);  // call the lambda/function associated with this key
        } else {
            throw std::runtime_error("Unknown input keyword: '" + key + "'");
        }
    }

    // Step 6: Determine the target calculation based on input.
    get_target(target);

}
//----------------------------------------------------------------------
// debugpgi
void Input::get_target(Target& target) {
    //
    // Assign the different targets.
    //
    if (!target.is_cutoff_present &&
        !target.omega_0           &&
        !target.integrate_density ) {

        throw std::runtime_error("Cutoff needed in input.");

    } else if (target.integrate_density &&
        (target.is_acceptor_density_present || target.is_donor_density_present || target.is_nanoparticle_present)) {
        
        throw std::runtime_error("You are requesting a cube integration with another type of calculation.");

    } else if (target.integrate_density) {
    
        target.mode = TargetMode::IntegrateCube;

    } else if (target.is_acceptor_density_present && 
               target.is_donor_density_present    && 
               !target.is_nanoparticle_present) {

        if (target.is_omega_0_present) target.calc_overlap_int = true; 
        if (!target.is_spectral_overlap_present) throw std::runtime_error("Aceptor-donor calculation requested but no spectral overlap in input.");

        target.mode = TargetMode::Acceptor_Donor; 

    } else {
        target.mode = TargetMode::None;
    }






//!
//     elseif(aceptor_density .and. nanoparticle .and. .not. donor_density) then
//!
//        target_%name_ = "aceptor_np"
//!
//     elseif(aceptor_density .and. nanoparticle .and. donor_density) then
//!
//        target_%name_ = "aceptor_np_donor"
//        if (target_%omega_0 < zero) call out_%error("Omega_0 cannot be negative")
//        if (target_%omega_0 > 1.0E-15) target_%calc_overlap_int = .true.
//        !if (target_%omega_0 < 1E-14) call out_%error("Aceptor-NP-donor calculation requested but no Omega_0 in input")
//!
//        if (target_%spectral_overlap < zero) call out_%error("Spectral overlap cannot be negative")
//        if (target_%spectral_overlap < 1E-14)&
//           call out_%error("Aceptor-NP-donor calculation requested but no spectral overlap in input")




//!
//!    Create folder to store debug quantities
//!
//     if (target_%debug.ge.1) then
//        if (file_exists("debug")) call execute_command_line("rm -rf debug")
//        call execute_command_line("mkdir debug")
//     endif
//!
//!    assign the different targets
//!
//     elseif(integrate_cube .and. aceptor_density .or. &
//            integrate_cube .and. donor_density   .or. &
//            integrate_cube .and. nanoparticle) then
//!
//        call out_%error( "You are requesting a cube integration with other type of calculation")
//!
//     elseif(integrate_cube) then
//!
//        target_%name_ = "integrate_density"
//!
//     elseif(donor_density .and. aceptor_density .and. .not. nanoparticle) then
//!
//        target_%name_ = "aceptor_donor"
//        if (target_%omega_0 < zero) call out_%error("Omega_0 cannot be negative")
//        if (target_%omega_0 > 1.0E-15) target_%calc_overlap_int = .true.
//        !if (target_%omega_0 < 1E-14) call out_%error("Aceptor-donor calculation requested but no Omega_0 in input")
//!
//        if (target_%spectral_overlap < zero) call out_%error("Spectral overlap cannot be negative")
//        if (target_%spectral_overlap < 1E-14)&
//           call out_%error("Aceptor-donor calculation requested but no spectral overlap in input")
//!
//     elseif(aceptor_density .and. nanoparticle .and. .not. donor_density) then
//!
//        target_%name_ = "aceptor_np"
//!
//     elseif(aceptor_density .and. nanoparticle .and. donor_density) then
//!
//        target_%name_ = "aceptor_np_donor"
//        if (target_%omega_0 < zero) call out_%error("Omega_0 cannot be negative")
//        if (target_%omega_0 > 1.0E-15) target_%calc_overlap_int = .true.
//        !if (target_%omega_0 < 1E-14) call out_%error("Aceptor-NP-donor calculation requested but no Omega_0 in input")
//!
//        if (target_%spectral_overlap < zero) call out_%error("Spectral overlap cannot be negative")
//        if (target_%spectral_overlap < 1E-14)&
//           call out_%error("Aceptor-NP-donor calculation requested but no spectral overlap in input")
//!
//     else
//!
//        call out_%error( "You are requesting an unkown calculation")
//!
//     endif
//!




    //if (is_acceptor_density_present)
    //    target.acceptor_density_file = acceptor_density_file;

    //if (is_donor_density_present)
    //    target.donor_density_file = donor_density_file;

    //if (acceptor_density_rotate || acceptor_transdip_rotate)
    //    target.rotate_acceptor = true;

    //if (donor_density_rotate || donor_transdip_rotate)
    //    target.rotate_donor = true;

    //// Consistency checks
    //if (target.rotate_acceptor && rotation_axis.empty())
    //    throw std::runtime_error("Acceptor rotation requested but no rotation axis given.");

    //if (acceptor_transdip_rotate && !acceptor_align_with && !acceptor_density_rotate)
    //    throw std::runtime_error("Acceptor dipole rotation requires alignment vector or angle.");

    // ... etc., just like your Fortran checks
}
//----------------------------------------------------------------------
///
/// @brief Resolves a file path relative to the input file location.
/// @param relative_path Path from the input file (may be relative)
/// @return Absolute path as a string
///
std::string Input::resolve_relative_to_input(const std::string& relative_path) const {
    namespace fs = std::filesystem;

    // Get directory of the input file
    fs::path input_dir = fs::absolute(input_filename).parent_path();

    // Combine with the relative path and normalize
    fs::path full_path = fs::absolute(input_dir / relative_path);

    return full_path.string();
}
//----------------------------------------------------------------------
///
/// @brief Checks if a file exists and throws an error if it does not.
/// @param path The path to the file to check.
///
void Input::file_exists(const std::string& path) const {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("File " + path + " not found.");
    }
}
//----------------------------------------------------------------------
///
/// @brief Resolves a file path, checks its existence, and stores both raw and resolved paths.
/// @param raw_input The file name as written in the input file (may be relative)
/// @param input_field Reference to the variable that stores the raw input name
/// @param resolved_field Reference to the variable that stores the resolved full path
///
void Input::check_and_store_file(
    const std::string& raw_input,
    std::string& input_field,
    std::string& resolved_field) const {

    input_field = raw_input;
    std::string full_path = resolve_relative_to_input(raw_input);
    file_exists(full_path);
    resolved_field = full_path;
}
//----------------------------------------------------------------------
///
/// @brief Prints input file information to the output stream.
///
void Input::print_input_info(const Output& out, const Target& target) {
    const std::string indent = std::string(23, ' ');

        out.stream() << indent << "Input  File: " << target.input_filename << "\n";
        out.stream() << indent << "Output File: " << out.output_filename << "\n\n";
        out.stream() << indent << "OMP Threads: " << "1" << "\n\n "; // For the moment, running in serial
        out.stream() << out.sticks << "\n";
        out.stream() << "\n";

    switch (target.mode) {
        case TargetMode::IntegrateCube:
            out.stream() << indent << "Calculation --> Integrate Cube Density\n\n";
            out.stream() << indent << "Density File: " << target.density_file_integration_input << "\n\n";
            out.stream() << " " << out.sticks << "\n \n";
        break;

        case TargetMode::None:
        default:
            throw std::runtime_error("No valid calculation target specified in input.");
    }

}
//----------------------------------------------------------------------



