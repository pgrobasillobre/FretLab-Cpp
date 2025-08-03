#ifndef TARGET_HPP
#define TARGET_HPP

#include "enum.hpp"

#include <string>
#include <array>

struct Target {
    TargetMode mode = TargetMode::None;

    // Input files
    std::string density_file;
    std::string acceptor_density_file;
    std::string donor_density_file;

    // Numeric parameters
    double cutoff = 0.0;
    double omega_0 = 0.0;
    double spectral_overlap = 0.0;

    // Rotation, vectors
    double acceptor_rotation_angle = 0.0;
    std::array<double, 3> acceptor_transdip{};
    std::array<double, 3> acceptor_ref_vector{};
    std::array<double, 3> donor_transdip{};
    std::array<double, 3> donor_ref_vector{};
    std::string rotation_axis;

    // Flags
    bool rotate_acceptor = false;
    bool rotate_donor = false;
    bool align_acceptor = false;
    bool align_donor = false;

    int debug_level = 0;
};

#endif // TARGET_HPP

