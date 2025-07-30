#include "algorithm.hpp"  
#include "input.hpp"
#include "density.hpp"

#include <iostream>

/// @brief Constructor for Algorithm.
Algorithm::Algorithm() {} 

//----------------------------------------------------------------------
/// @brief Integrates the density of the input cube file.
void Algorithm::integrate_density(const Input& inp) {

    density.read_density(inp.density_file_integration, false, "");
}
//----------------------------------------------------------------------