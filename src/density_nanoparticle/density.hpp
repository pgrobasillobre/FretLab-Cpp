#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "target.hpp"

#include <string>
#include <vector>
#include <array>

class Output; // Forward declaration
//----------------------------------------------------------------------
///
/// @class Density
/// @brief Represents and processes electron density data from a cube file.
///
class Density {
public:

    // Descriptive headers from the cube file
    std::string str1, str2;

    // Grid and atom metadata
    int natoms = 0, nx = 0, ny = 0, nz = 0;
    int nelectrons = 0;
    int n_points_reduced = 0;

    // Cube file information
    std::vector<int> atomic_number;              ///< Atomic numbers of the atoms
    std::vector<std::string> atomic_label;       ///< Element symbols
    std::vector<double> atomic_charge;           ///< Atomic charges
    std::vector<double> x, y, z;                 ///< Atomic positions

    double xmin = 0.0, ymin = 0.0, zmin = 0.0;
    std::array<double, 3> dx{}, dy{}, dz{};      ///< Voxel vectors in each direction

    std::vector<std::vector<std::vector<double>>> rho;

    std::vector<double> rho_reduced;
    std::vector<std::array<double, 3>> xyz;

    double maxdens = 0.0, volume = 0.0;
    
    std::array<double, 3> geom_center = {0.0, 0.0, 0.0}; ///< Geometric center of the density
    std::array<double, 3> geom_center_mol = {0.0, 0.0, 0.0}; ///< Geometric center of the molecule

    double integral = 0.0;  ///< Integral of the density over the full grid

    // Functions to handle density data
    /**
     * @brief Reads cube file and loads density data.
     * @param filepath Path to the cube file.
     * @param what_dens Specifies the density role (e.g., "acceptor", "donor").
     */
    void read_density(Target& target, const Output& out, const std::string& what_dens = "");


    /** 
     * @brief Integrates the full density grid.
     */
    void int_density();


private:

    /**
     * @brief Maps atomic number to corresponding element label.
     * @param Z Atomic number
     * @return Element symbol as a string
     */
    std::string map_atomic_number_to_label(int Z) const;


    ///
    /// @brief Rotates the transition dipole moment based on the reference vector.
    ///
    void rotate_transition_dipole(Target &target, const Output & out, const std::string& what_dens);

    ///
    /// @brief Compute angle between two vectors.
    ///
    double compute_angle_between_vectors(const std::array<double, 3>& vec1, const std::array<double, 3>& vec2) const;

    ///
    /// @brief Rotate vector based on angle and rotation axis.
    ///
    std::array<double, 3> rotate_vector(const std::array<double, 3>& vec, const double angle, const std::string& axis) const;

};

#endif // DENSITY_HPP
