#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "target.hpp"

#include <string>
#include <vector>
#include <array>

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
    std::array<double, 3> geom_center{}, geom_center_mol{};

    double integral = 0.0;  ///< Integral of the density over the full grid

    // Functions to handle density data
    /**
     * @brief Reads cube file and loads density data.
     * @param filepath Path to the cube file.
     * @param rotate Whether to apply coordinate rotation (not yet implemented).
     * @param what_dens Specifies the density role (e.g., "acceptor", "donor").
     */
    //void read_density(const std::string& filepath, bool rotate = false, const std::string& what_dens = "");
    void read_density(const Target& target, bool rotate = false, const std::string& what_dens = "");


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

};

#endif // DENSITY_HPP
