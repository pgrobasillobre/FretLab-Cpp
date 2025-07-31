#include "density.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>

///
/// @brief Returns the element label (e.g., "H", "C") for a given atomic number.
///
std::string Density::map_atomic_number_to_label(int Z) const {
    static const std::vector<std::string> periodic_table = {
        "", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne",
        "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca",
        "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
        "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
        "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
        "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
        "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
        "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg"
    };
    return (Z > 0 && Z < (int)periodic_table.size()) ? periodic_table[Z] : "X";
}

///
/// @brief Loads a cube file and initializes the density grid and atomic data.
///
void Density::read_density(const std::string& filepath, bool rotate, const std::string& what_dens) {
    std::ifstream infile(filepath);
    if (!infile) {
        throw std::runtime_error("File: " + filepath + "not found.");
    }

    // Skip header lines
    std::getline(infile, str1);
    std::getline(infile, str2);

    // Read grid and origin info
    infile >> natoms >> xmin >> ymin >> zmin;
    infile >> nx >> dx[0] >> dx[1] >> dx[2];
    infile >> ny >> dy[0] >> dy[1] >> dy[2];
    infile >> nz >> dz[0] >> dz[1] >> dz[2];

    // Ensure voxel matrix is diagonal. Compute voxel volume.
    if (dx[1] != 0.0 || dx[2] != 0.0 ||
        dy[0] != 0.0 || dy[2] != 0.0 ||
        dz[0] != 0.0 || dz[1] != 0.0) {
        throw std::runtime_error("Cube file conflict: dx, dy, dz matrix is not diagonal.");
    }
    volume = dx[0] * dy[1] * dz[2];

    // Initialize atom vectors
    atomic_number.resize(natoms);
    atomic_label.resize(natoms);
    atomic_charge.resize(natoms);
    x.resize(natoms);
    y.resize(natoms);
    z.resize(natoms);

    // Read atom block
    nelectrons = 0;
    for (int i = 0; i < natoms; ++i) {
        infile >> atomic_number[i] >> atomic_charge[i] >> x[i] >> y[i] >> z[i];
        nelectrons += atomic_number[i];
        atomic_label[i] = map_atomic_number_to_label(atomic_number[i]);
    }

    // Allocate 3D density grid
    rho.resize(nx, std::vector<std::vector<double>>(ny, std::vector<double>(nz)));

    // Read density values, weight by voxel volume, and find maximum density value
    maxdens = 0.0;
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            for (int k = 0; k < nz; ++k) {
                infile >> rho[i][j][k];
                rho[i][j][k] *= volume;
                maxdens = std::max(maxdens, std::abs(rho[i][j][k]));
            }
        }
    }

    // NOTE: reduction, geometry center, and rotation will be added later

    infile.close();
}

///
/// @brief Integrates the full density grid by summing all density values.
///
void Density::int_density() {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            for (int k = 0; k < nz; ++k) {
                integral += rho[i][j][k];
            }
        }
    }
}
 
