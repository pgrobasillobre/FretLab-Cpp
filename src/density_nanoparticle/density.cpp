#include "density.hpp"
#include "parameters.hpp"
#include "target.hpp"

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
//void Density::read_density(const std::string& filepath, bool rotate, const std::string& what_dens) {
void Density::read_density(const Target& target, bool rotate, const std::string& what_dens) {


    // Check density file final purpose: cube integration, acceptor, or donor density.
    std::string filepath;

    if (what_dens=="Cube"){
       filepath = target.density_file_integration;
    } else if (what_dens=="Acceptor"){
       filepath = target.acceptor_density_file;
    } else if (what_dens=="Donor"){
       filepath = target.donor_density_file;
    } else {
        throw std::runtime_error("Unknown density file mode to read.");
    }

    // Check file existance.
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
    // 
    // debugpgi
    // Save reducde density of the cube, and calculate associated coordinates
    //
    if (!target.integrate_density) {
        rho_reduced.resize(Parameters::ncellmax);
        xyz.resize(Parameters::ncellmax);
    
        double x_tmp = 0.0, y_tmp = 0.0, z_tmp = 0.0;
    
        for (int i = 0; i < nx; ++i) {
            x_tmp = xmin + dx[1] * i;
    
            for (int j = 0; j < ny; ++j) {
                y_tmp = ymin + dy[2] * j;
    
                for (int k = 0; k < nz; ++k) {
                    z_tmp = zmin + dz[3] * k;
    
                    if (std::abs(rho[i][j][k]) > maxdens * target.cutoff || target.calc_overlap_int) {
                        ++n_points_reduced;  // n_points_reduced initialized to -1
    
                        if (n_points_reduced > Parameters::ncellmax) throw std::runtime_error("Increase cutoff. " + what_dens + " density file too big.");
                        
    
                        rho_reduced[n_points_reduced] = rho[i][j][k];
    
                        xyz[n_points_reduced][0] = x_tmp; 
                        xyz[n_points_reduced][1] = y_tmp;
                        xyz[n_points_reduced][2] = z_tmp;
                    }
                }
            }
        }
    }


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
 
