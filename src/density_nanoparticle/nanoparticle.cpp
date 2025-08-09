#include "nanoparticle.hpp"
#include "target.hpp"
#include "parameters.hpp"
#include "string_manipulation.hpp"

#include <iostream>
#include <fstream>

///
/// @brief Loads a cube file and initializes the density grid and atomic data.
///
// void Density::read_density(const std::string& filepath, bool rotate, const std::string& what_dens) {
void Nanoparticle::read_nanoparticle(const Target &target)
{
  // Check file existance.
  std::string filepath = target.nanoparticle_file;

  std::ifstream infile(filepath);
  if (!infile)
  {
    throw std::runtime_error("File: " + filepath + "not found.");
  }

  bool is_fret_quantities_present = false;
  is_fret_quantities_present = go_to_string(infile, Parameters::fret_start);

  std::cout << "FRET quantities present: " << std::boolalpha << is_fret_quantities_present << std::endl;
}
