#include "output.hpp"
#include "parameters.hpp"
#include "integrals.hpp"
#include "nanoparticle.hpp"

#include <iostream>
#include <filesystem>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <ostream>
#include <numeric>
#include <cmath>

/// @brief Constructor for Output.
Output::Output() {}

//----------------------------------------------------------------------
///
/// @brief Creates the output filename based on the input file name.
///
void Output::out_file_fill(const std::string &in_file)
{

    /// @brief Generate the output filename based on the input filename.
    ///
    /// This function removes the last 4 characters of the input filename
    /// (typically ".inp") and replaces them with ".log". For example:
    /// - input: "test.inp"
    /// - output: "test.log"
    ///
    /// It also prints the new output filename to the console.
    ///
    /// @param in_file Name of the input file

    output_filename = in_file.substr(0, in_file.size() - 4) + ".log";
}
//----------------------------------------------------------------------
///
/// @brief Opens the output file for writing.
///
void Output::open()
{
    log_stream.open(output_filename, std::ios::out);
    if (!log_stream.is_open())
    {
        throw std::runtime_error("Failed to open output file: " + output_filename);
    }
}
//----------------------------------------------------------------------
///
/// @brief Gets the internal output stream.
///
std::ofstream &Output::stream() const
{
    return const_cast<std::ofstream &>(log_stream);
}
//----------------------------------------------------------------------
///
/// @brief Closes the output file stream.
///
void Output::close()
{
    if (log_stream.is_open())
    {
        log_stream.close();
    }
}
//----------------------------------------------------------------------
///
/// @brief Prints FretLab banner.
///
void Output::print_banner()
{
    const std::string indent = std::string(20, ' ');

    log_stream << " " << sticks << "\n \n";

    log_stream << indent << "    ______          __  __          __  \n";
    log_stream << indent << "   / ____/_______  / /_/ /   ____ _/ /_ \n";
    log_stream << indent << "  / /_  / ___/ _  / __/ /   / __ `/ __ |\n";
    log_stream << indent << " / __/ / /  /  __/ /_/ /___/ /_/ / /_/ /\n";
    log_stream << indent << "/_/   /_/   |___/ __/_____/__,_/_.___/  \n";
    log_stream << indent << "                                        \n";

    log_stream << " \n " << sticks << "\n \n";
    log_stream << std::string(25, ' ') << "Program by Pablo Grobas Illobre\n \n";
    log_stream << " " << sticks << "\n \n";
    log_stream.flush();
}
//----------------------------------------------------------------------
///
/// @brief Fills the output file with density information.
///
void Output::print_density(const std::string &filepath, const Density &cube, std::optional<std::string> header)
{

    // Extract the filename from the full path
    std::string filename = std::filesystem::path(filepath).filename().string();

    if (header.has_value())
    {
        log_stream << header.value() << "\n \n";
    }
    else
    {
        log_stream << std::string(29, ' ') << "Density Information                    \n \n";
    }
    log_stream << " " << sticks << "\n \n";
    log_stream << std::string(3, ' ') << "Density File: " << filename << "\n \n";
    log_stream << std::string(3, ' ') << "Density Grid (CUBE format): \n \n";

    print_formatted_line1(log_stream, cube.natoms, cube.xmin, cube.ymin, cube.zmin);
    print_formatted_line1(log_stream, cube.nx, cube.dx[0], cube.dx[1], cube.dx[2]);
    print_formatted_line1(log_stream, cube.ny, cube.dy[0], cube.dy[1], cube.dy[2]);
    print_formatted_line1(log_stream, cube.nz, cube.dz[0], cube.dz[1], cube.dz[2]);
    log_stream << " \n";
    log_stream << "     Total number of grid points: " << cube.nx * cube.ny * cube.nz << "\n";
    if (!header.has_value())
    {
        log_stream << " \n"; // Integrate cube case
    }
    else if (header.has_value())
    {
        log_stream << "     ---> Reduced density points: " << cube.n_points_reduced << "\n \n"; // Integrate cube case
    }

    // If (target_%name_ .ne. "integrate_density") then
    //    Write(out_%iunit,*)     "    ---> Reduced density points:", cube%n_points_reduced
    // Endif

    log_stream << std::string(3, ' ') << "Associated molecular coordinates (Å): \n \n";
    for (int i = 0; i < cube.natoms; ++i)
    {
        print_formatted_line2(log_stream, std::string(cube.atomic_label[i]),
                              cube.x[i] * Parameters::ToAng,
                              cube.y[i] * Parameters::ToAng,
                              cube.z[i] * Parameters::ToAng);
    }

    if (cube.integral > 0.0)
    {
        log_stream << " \n";
        log_stream << "    ============================================================\n";
        log_stream << "     Integrated electron density -->    " << std::fixed << std::setprecision(14) << cube.integral << "\n";
        log_stream << "    ============================================================\n";
    }

    log_stream << " \n " << sticks << "\n\n";
}
//----------------------------------------------------------------------
///
///
///
void Output::print_nanoparticle(const Nanoparticle &np)
{
    log_stream << std::string(23, ' ') << "Nanoparticle Model   : " << np.nanoparticle_model << "\n\n";
    log_stream << sticks << "\n\n";
    log_stream << std::string(28, ' ') << "Nanoparticle Geometry (Å)                    \n \n";
    log_stream << " " << sticks << "\n \n";
    log_stream << std::string(13, ' ') << "Atom" << std::string(15, ' ') << "X" << std::string(19, ' ') << "Y" << std::string(19, ' ') << "Z" << "\n";
    log_stream << " " << sticks << "\n \n";

    // Print nanoparticle properties
    for (int i = 0; i < np.natoms; ++i)
    {
        print_formatted_line3(log_stream, std::string("Xx"),
                              np.xyz[i][0] * Parameters::ToAng,
                              np.xyz[i][1] * Parameters::ToAng,
                              np.xyz[i][2] * Parameters::ToAng);
    }

    log_stream << " \n " << sticks << "\n\n";
}
//----------------------------------------------------------------------
///
/// @brief Prints a formatted line with cube information to the output stream.
///
void Output::print_formatted_line1(std::ostream &out, int i, double a, double b, double c)
{
    char line[100];
    std::snprintf(line, sizeof(line), "   %5d %15.7E %15.7E %15.7E\n", i, a, b, c);
    out << line;
}
// ----------------------------------------------------------------------
///
/// @brief Prints a formatted line with atom information to the output stream.
//
void Output::print_formatted_line2(std::ostream &out, const std::string atom, double x, double y, double z)
{
    char line[100];
    std::snprintf(line, sizeof(line), "       %-2s  %12.6f  %12.6f  %12.6f\n", atom.c_str(), x, y, z);
    out << line;
}
// ----------------------------------------------------------------------
///
/// @brief Prints a formatted line with nanoparticle information to the output stream.
//
void Output::print_formatted_line3(std::ostream &out, const std::string atom, double x, double y, double z)
{
    char line[100];
    std::snprintf(line, sizeof(line), "              %-2s  %19.6f %19.6f %19.6f\n", atom.c_str(), x, y, z);
    out << line;
}
//----------------------------------------------------------------------
///
/// @brief Prints the results of the integrals to the output stream.
///
void Output::print_results_integrals(const Target &target, const Integrals &integrals)
{
    std::array<double, 2> v_tot = {0.0, 0.0};
    double v_mod = 0.0;

    // Print header
    log_stream << std::string(36, ' ') << "RESULTS\n\n";
    log_stream << " " << sticks << " \n\n";

    switch (target.mode)
    {

    case TargetMode::Acceptor_Donor:

        log_stream << std::string(5, ' ') << "Acceptor-Donor Coulomb  : " << std::fixed << std::setw(25) << std::setprecision(16) << integrals.coulomb_acceptor_donor << "  a.u.\n";
        if (target.calc_overlap_int)
        {
            log_stream << std::string(5, ' ') << "Acceptor-Donor Overlap  : " << std::fixed << std::setw(25) << std::setprecision(16) << integrals.overlap_acceptor_donor << "  a.u.\n";
        }
        v_tot[0] = integrals.coulomb_acceptor_donor + integrals.overlap_acceptor_donor;

        v_mod = std::sqrt(std::inner_product(v_tot.begin(), v_tot.end(), v_tot.begin(), 0.0));

        log_stream << std::string(37, ' ') << std::string(26, '-') << "\n";
        log_stream
            << std::string(5, ' ') << "Total Potential         : " << std::fixed << std::setw(25) << std::setprecision(16) << v_tot[0] << "  a.u.\n\n";
        log_stream << std::string(5, ' ') << "Total Potential Modulus : " << std::fixed << std::setw(25) << std::setprecision(16) << v_mod << "  a.u.\n\n";

        log_stream << std::string(5, ' ') << "Keet :" << std::fixed << std::setw(25) << std::setprecision(16)
                   << 2.0 * Parameters::pi * (v_mod * v_mod) * target.spectral_overlap << "  a.u.\n\n";

        log_stream << " " << sticks << "\n\n";
        log_stream.flush();

        break;

    case TargetMode::Acceptor_NP:

        log_stream << std::string(5, ' ') << "Acceptor-NP Interaction : " << std::fixed << std::setw(25) << std::setprecision(16) << integrals.overlap_acceptor_nanoparticle[0] << " + " << integrals.overlap_acceptor_nanoparticle[1] << " i  a.u.\n\n";
        log_stream << " " << sticks << "\n\n";
        log_stream.flush();
        break;

    case TargetMode::None:
    default:
        throw std::runtime_error("No valid calculation target specified in input.");
    }
}
