#ifndef TARGET_HPP
#define TARGET_HPP

#include "enum.hpp"

#include <string>
#include <array>

struct Target {
    TargetMode mode = TargetMode::None;

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
    std::string donor_density_input_file; /// File for donor density as named in input

    // Nanoparticle
    bool is_nanoparticle_present = false;

    std::string nanoparticle_file; ///< File for nanoparticle (full path)
    std::string nanoparticle_input_file; /// File for nanoparticle as named in input

    // Target + other options
    TargetMode target_mode = TargetMode::None;  ///< Selected calculation target

    bool integrate_density = false;

    bool calc_overlap_int = false;

    bool is_cutoff_present = false;
    double cutoff = 0.0; 

    bool is_omega_0_present = false;
    double omega_0 = 0.0; 


    bool is_spectral_overlap_present = false;
    double spectral_overlap = 0.0;

    bool is_debug_present;
    int debug = 0;

};

#endif // TARGET_HPP

