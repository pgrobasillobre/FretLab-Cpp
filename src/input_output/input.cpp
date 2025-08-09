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
#include <algorithm>
#include <stdexcept>
#include <cstdlib>

#include <omp.h>

namespace fs = std::filesystem;

//----------------------------------------------------------------------
///
/// @brief Constructor: Initializes the default input file name to "input.inp"
///
Input::Input() : input_filename("input.inp") {} // default name
//----------------------------------------------------------------------
///
/// @brief Delegates to private function for parsing command-line arguments.
///
void Input::get_arguments(int argc, char *argv[], Output &out, Target &target)
{
    try
    {
        // Parse filename etc.
        parse_arguments(argc, argv, out);
        target.input_filename = input_filename;

#ifdef _OPENMP
        bool saw_omp_flag = false;
        target.n_threads_OMP = 1; // placeholder until we decide
#else
        target.n_threads_OMP = 1; // always 1 without OpenMP
#endif

        // Scan for -omp N (allowed anywhere)
        for (int i = 1; i < argc; ++i)
        {
            std::string a = argv[i];
            if (a == "-omp")
            {
                if (i + 1 >= argc)
                {
                    throw std::runtime_error("Missing value for -omp: you must specify an integer after -omp");
                }

#ifdef _OPENMP
                saw_omp_flag = true;
                str_manipulation.string_to_int(argv[i + 1], target.n_threads_OMP);
                if (target.n_threads_OMP < 1)
                {
                    throw std::runtime_error("Value for -omp must be >= 1");
                }
                else if (target.n_threads_OMP > omp_get_max_threads())
                {
                    target.n_threads_OMP = omp_get_max_threads();
                }
#else
                // Optional: warn user it’s ignored when OpenMP is off
                std::cout << "\n Warning: -omp ignored; binary built without OpenMP.\n"
                          << std::endl;
#endif
                ++i; // skip value
            }
        }

#ifdef _OPENMP
        // If no -omp provided, use all available threads
        if (!saw_omp_flag)
        {
            target.n_threads_OMP = omp_get_max_threads();
        }
        omp_set_num_threads(std::max(1, target.n_threads_OMP));
#endif
    }
    catch (const std::exception &e)
    {
        // Log the error before stopping
        std::cout << "\n ERROR: " << e.what() << "\n"
                  << std::endl;
        throw; // rethrow so main() or caller can terminate the program
    }
}
//----------------------------------------------------------------------
///
/// @brief Parses user-supplied command-line arguments and sets the input filename.
/// Handles three cases:
/// 1. One argument (input filename)
/// 2. No arguments (ask user to type filename)
/// 3. Too many arguments (throws error -- TODO)
///
void Input::parse_arguments(int argc, char *argv[], Output &out)
{
    input_filename.clear();

    // No args: interactive prompt
    if (argc == 1)
    {
        std::cout << "   Type the input filename (e.g. filename.inp): ";
        std::getline(std::cin, input_filename);
        out.out_file_fill(input_filename);
        return;
    }

    // Walk args, allow: program [input.inp] [-omp N] in any order
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "-omp")
        {
            // skip value here; get_arguments will process it
            if (i + 1 >= argc)
                throw std::runtime_error("Missing value for -omp");
            ++i;
            continue;
        }
        if (!a.empty() && a[0] == '-')
        {
            throw std::runtime_error("Unknown option: " + a);
        }
        if (input_filename.empty())
        {
            input_filename = a; // first non-option = filename
        }
        else
        {
            throw std::runtime_error("Too many arguments. Provide only one input file (plus optional -omp N).");
        }
    }

    if (input_filename.empty())
    {
        throw std::runtime_error("No input file provided. Usage: program input.inp [-omp N]");
    }

    out.out_file_fill(input_filename); // create output filename(s)
}
//----------------------------------------------------------------------
///
/// @brief Check that files exists and has the supported extension (.inp).
///
void Input::check_input_file(const Output &out)
{
    std::ifstream file(input_filename);
    if (!file)
    {
        throw std::runtime_error("File " + input_filename + " does not exist. Check the file name and path.");
    }

    // Creates new string variable (lowercase) to check extension
    std::string lower_filename = input_filename;
    std::transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);

    // Check file extension is .inp
    if (lower_filename.size() < 4 || lower_filename.substr(lower_filename.size() - 4) != ".inp")
    {
        throw std::runtime_error("File " + input_filename + " does not have the supported extension (.inp).");
    }
}
//----------------------------------------------------------------------
///
/// @brief Reads the input file and parses its content.
///
void Input::read(Target &target)
{

    // Open the input file.
    std::ifstream file(input_filename);
    file_exists(input_filename); // Check file existence

    // Step 1: Create a dispatch table (a keyword-to-function map)
    std::unordered_map<std::string, std::function<void(const std::string &)>> handlers;

    // Step 2: Define how each keyword should be handled
    // ========
    handlers["integrate cube file"] = [&](const std::string &value)
    {
        check_and_store_file(value, target.density_file_integration_input, target.density_file_integration);
        target.integrate_density = true;
    };
    // ========
    handlers["acceptor density"] = [&](const std::string &value)
    {
        check_and_store_file(value, target.acceptor_density_input_file, target.acceptor_density_file);
        target.is_acceptor_density_present = true;
    };
    // ========
    handlers["donor density"] = [&](const std::string &value)
    {
        check_and_store_file(value, target.donor_density_input_file, target.donor_density_file);
        target.is_donor_density_present = true;
    };
    // ========
    handlers["nanoparticle"] = [&](const std::string &value)
    {
        check_and_store_file(value, target.nanoparticle_input_file, target.nanoparticle_file);
        target.is_nanoparticle_present = true;
    };
    // ========
    handlers["cutoff"] = [&](const std::string &value)
    {
        str_manipulation.string_to_float(value, target.cutoff);
        if (target.cutoff < 0.0)
            throw std::runtime_error("Cutoff cannot be negative.");
        target.is_cutoff_present = true;
    };
    // ========
    handlers["spectral overlap"] = [&](const std::string &value)
    {
        str_manipulation.string_to_float(value, target.spectral_overlap);
        target.is_spectral_overlap_present = true;
        if (target.spectral_overlap < 0.0)
            throw std::runtime_error("Spectral overlap cannot be negative.");
    };
    // ========
    handlers["debug"] = [&](const std::string &value)
    {
        str_manipulation.string_to_int(value, target.debug);
        target.is_debug_present = true;
        if (target.debug < 0.0)
            throw std::runtime_error("Debug mode must be equal or higher than 0.");
    };
    // ========
    handlers["omega_0"] = [&](const std::string &value)
    {
        str_manipulation.string_to_float(value, target.omega_0);
        target.is_omega_0_present = true;
        if (target.omega_0 < 0.0)
            throw std::runtime_error("omega_0 cannot be negative");
    };
    // ========

    std::string line;
    // Step 3: Read input file line-by-line
    while (std::getline(file, line))
    {
        // Trim leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines or comment lines (starting with '#' or '!')
        if (line.empty() || line[0] == '#' || line[0] == '!')
            continue;

        // Step 4: Split line into keyword and value using ':'
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            continue; // no colon found — skip or optionally throw an error
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
        if (handlers.contains(key))
        {
            handlers[key](value); // call the lambda/function associated with this key
        }
        else
        {
            throw std::runtime_error("Unknown input keyword: '" + key + "'");
        }
    }

    // Step 6: Determine the target calculation based on input.
    get_target(target);
}
//----------------------------------------------------------------------
// debugpgi
void Input::get_target(Target &target)
{
    //
    // Create debug folder
    //
    if (target.is_debug_present && target.debug > 0)
    {
        if (fs::exists("debug"))
        {
            fs::remove_all("debug"); // Equivalent of 'rm -rf debug'
        }
        fs::create_directory("debug");
    }
    //
    // Assign the different targets.
    //
    if (!target.is_cutoff_present &&
        !target.omega_0 &&
        !target.integrate_density)
    {

        throw std::runtime_error("Cutoff needed in input.");
    }
    else if (target.integrate_density &&
             (target.is_acceptor_density_present || target.is_donor_density_present || target.is_nanoparticle_present))
    {

        throw std::runtime_error("You are requesting a cube integration with another type of calculation.");
    }
    else if (target.integrate_density)
    {

        target.mode = TargetMode::IntegrateCube;
    }
    else if (target.is_acceptor_density_present &&
             target.is_donor_density_present &&
             !target.is_nanoparticle_present)
    {

        if (target.is_omega_0_present)
            target.calc_overlap_int = true;
        if (!target.is_spectral_overlap_present)
            throw std::runtime_error("Aceptor-donor calculation requested but no spectral overlap in input.");

        target.mode = TargetMode::Acceptor_Donor;
    }
    else if (target.is_acceptor_density_present &&
             target.is_nanoparticle_present &&
             !target.is_donor_density_present)
    {

        target.mode = TargetMode::Acceptor_NP;
    }
    else if (target.is_acceptor_density_present &&
             target.is_nanoparticle_present &&
             target.is_donor_density_present)
    {

        if (target.is_omega_0_present)
            target.calc_overlap_int = true;
        if (!target.is_spectral_overlap_present)
            throw std::runtime_error("Aceptor-NP-donor calculation requested but no spectral overlap in input.");

        target.mode = TargetMode::Acceptor_NP_Donor;
    }
    else
    {
        target.mode = TargetMode::None;
    }
}
//----------------------------------------------------------------------
///
/// @brief Resolves a file path relative to the input file location.
/// @param relative_path Path from the input file (may be relative)
/// @return Absolute path as a string
///
std::string Input::resolve_relative_to_input(const std::string &relative_path) const
{

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
void Input::file_exists(const std::string &path) const
{
    std::ifstream file(path);
    if (!file)
    {
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
    const std::string &raw_input,
    std::string &input_field,
    std::string &resolved_field) const
{

    input_field = raw_input;
    std::string full_path = resolve_relative_to_input(raw_input);
    file_exists(full_path);
    resolved_field = full_path;
}
//----------------------------------------------------------------------
///
/// @brief Prints input file information to the output stream.
///
void Input::print_input_info(const Output &out, const Target &target)
{
    const std::string indent = std::string(23, ' ');

    out.stream() << indent << "Input  File: " << target.input_filename << "\n";
    out.stream() << indent << "Output File: " << out.output_filename << "\n\n";
    out.stream() << indent << "OMP Threads: " << target.n_threads_OMP << "\n\n ";
    out.stream() << out.sticks << "\n";
    out.stream() << "\n";

    switch (target.mode)
    {
    case TargetMode::IntegrateCube:
        out.stream() << indent << "Calculation --> Integrate Cube Density\n\n";
        out.stream() << indent << "Density File: " << target.density_file_integration_input << "\n\n";
        out.stream() << " " << out.sticks << "\n \n";
        break;

    case TargetMode::Acceptor_Donor:
        out.stream() << indent << "Calculation --> Acceptor - Donor\n\n";
        out.stream() << indent << "Acceptor Density File: " << target.acceptor_density_input_file << "\n";
        out.stream() << indent << "Donor    Density File: " << target.donor_density_input_file << "\n\n";
        if (!target.calc_overlap_int)
        {
            out.stream() << indent << "Overlap Integral     : No\n";
            out.stream() << indent << "Cutoff               : " << target.cutoff << "   a.u.\n";
            out.stream() << indent << "Spectral Overlap     : " << target.spectral_overlap << "   a.u.\n\n";
        }
        else
        {
            out.stream() << indent << "Overlap Integral     : Yes\n";
            out.stream() << indent << "Cutoff               : No\n";
            out.stream() << indent << "Spectral Overlap     : " << target.spectral_overlap << "   a.u.\n";
            out.stream() << indent << "Omega_0              : " << target.omega_0 << "   a.u.\n\n";
        }
        out.stream() << " " << out.sticks << "\n \n";

        break;

    case TargetMode::Acceptor_NP:
        out.stream() << indent << "Calculation --> Acceptor - NP\n\n";
        out.stream() << indent << "Acceptor Density File: " << target.acceptor_density_input_file << "\n";
        out.stream() << indent << "Nanoparticle File    : " << target.nanoparticle_input_file << "\n\n";

        out.stream() << indent << "Overlap Integral     : No\n";
        out.stream() << indent << "Cutoff               : " << target.cutoff << "   a.u.\n\n";
        out.stream() << " " << out.sticks << "\n \n";

        if (target.calc_overlap_int)
            throw std::runtime_error("Overlap integral can't be computed for Acceptor - NP option.");

        break;

    case TargetMode::Acceptor_NP_Donor:
        out.stream() << indent << "Calculation --> Acceptor - NP - Donor\n\n";
        out.stream() << indent << "Acceptor Density File: " << target.acceptor_density_input_file << "\n";
        out.stream() << indent << "Donor    Density File: " << target.donor_density_input_file << "\n";
        out.stream() << indent << "Nanoparticle File    : " << target.nanoparticle_input_file << "\n\n";
        if (!target.calc_overlap_int)
        {
            out.stream() << indent << "Overlap Integral     : No\n";
            out.stream() << indent << "Cutoff               : " << target.cutoff << "   a.u.\n";
            out.stream() << indent << "Spectral Overlap     : " << target.spectral_overlap << "   a.u.\n\n";
        }
        else
        {
            out.stream() << indent << "Overlap Integral     : Yes\n";
            out.stream() << indent << "Cutoff               : No\n";
            out.stream() << indent << "Omega_0              : " << target.omega_0 << "   a.u.\n";
            out.stream() << indent << "Spectral Overlap     : " << target.spectral_overlap << "   a.u.\n\n";
        }
        out.stream() << " " << out.sticks << "\n \n";

        break;

    case TargetMode::None:
    default:
        throw std::runtime_error("No valid calculation target specified in input.");
    }
}
//----------------------------------------------------------------------
