#ifndef NANOPARTICLE_HPP
#define NANOPARTICLE_HPP

#include "target.hpp"

#include <string>
#include <vector>
#include <array>

class Output; // Forward declaration
//----------------------------------------------------------------------
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

  /// @brief Loads nanoparticle coordinates and charges (optional: + dipoles) 
  void read_nanoparticle(const Target &target, const Output &out);

private:

  /// @brief  Rotate nanoparticle coordinate and dipoles
  void rotate_np_coords_and_dipoles(const Target &target, const Output &out);

  /// @brief Rotate nanoparticle dipoles, if present
  void rotate_np_dipoles(const double angle, const std::string& axis);

  /// @brief Rotate nanoparticle xyz coordinates based on angle and rotation axis.
  std::vector<std::array<double, 3>> rotate_np_coords(const double angle, const std::string& axis) const;

};

#endif // NANOPARTICCLE_HPP
