#ifndef TARGET_HPP
#define TARGET_HPP

#include "enum.hpp"

#include <string>
#include <array>

struct Target
{

    TargetMode mode = TargetMode::None; ///< Selected calculation target

    // Density integration
    std::string input_filename;
    std::string density_file_integration;       ///< File for density integration (full path)
    std::string density_file_integration_input; /// File for density integration as named in input

    // Acceptor
    bool is_acceptor_density_present = false;
    bool is_acceptor_transition_dipole_present = false;
    bool is_acceptor_transition_dipole_align_present = false;
    bool rotate_acceptor = false;

    std::string acceptor_density_file;       ///< File for acceptor density (full path)
    std::string acceptor_density_input_file; /// File for acceptor density as named in input

    std::array<double, 3> acceptor_transdip = {0.0, 0.0, 0.0}; ///< Transition dipole moment of the acceptor
    std::array<double, 3> acceptor_transdip_rot = {0.0, 0.0, 0.0}; ///< Rotated transition dipole moment of the donor
    std::array<double, 3> acceptor_ref_vector = {0.0, 0.0, 0.0}; ///< Reference vector for acceptor alignment

    double acceptor_density_rotation_angle = 0.0;
    double acceptor_density_rotation_angle_check = 0.0;


    // Donor
    bool is_donor_density_present = false;
    bool is_donor_transition_dipole_present = false;
    bool is_donor_transition_dipole_align_present = false;
    bool rotate_donor = false;

    std::string donor_density_file;       ///< File for donor density (full path)
    std::string donor_density_input_file; /// File for donor density as named in input

    std::array<double, 3> donor_transdip = {0.0, 0.0, 0.0}; ///< Transition dipole moment of the donor
    std::array<double, 3> donor_transdip_rot = {0.0, 0.0, 0.0}; ///< Rotated transition dipole moment of the donor
    std::array<double, 3> donor_ref_vector = {0.0, 0.0, 0.0}; ///< Reference vector for donor alignment 

    double donor_density_rotation_angle = 0.0;
    double donor_density_rotation_angle_check = 0.0;


    // Nanoparticle
    bool is_nanoparticle_present = false;
    bool rotate_nanoparticle = false;

    std::string nanoparticle_file;       ///< File for nanoparticle (full path)
    std::string nanoparticle_input_file; /// File for nanoparticle as named in input

    // Target + other options
    TargetMode target_mode = TargetMode::None; ///< Selected calculation target

    std::string rotation_axys;
    bool is_rotation_axys_present = false;

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

    int n_threads_OMP = 1;
};

#endif // TARGET_HPP
