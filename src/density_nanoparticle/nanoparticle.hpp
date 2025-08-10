#ifndef NANOPARTICLE_HPP
#define NANOPARTICLE_HPP

#include "target.hpp"

#include <string>
#include <vector>
#include <array>

///
/// @class Nanoparticle
/// @brief Represents and processes nanoparticle's charges and dipoles data from file.
///
class Nanoparticle
{
public:

  int natoms = 0;

  bool charges = false, charges_and_dipoles = false;

  std::string nanoparticle_model;

  std::array<double, 3> geom_center; 

  std::vector<std::array<double, 2>> q;    // Charges with real + imaginary part
  std::vector<std::array<double, 6>> mu;   // Dipoles with 3 components each for real + imaginary part

  std::vector<std::array<double, 3>> xyz;  // XYZ coordinates

  // void read_density(const std::string& filepath, bool rotate = false, const std::string& what_dens = "");
  void read_nanoparticle(const Target &target);
};

#endif // NANOPARTICCLE_HPP
