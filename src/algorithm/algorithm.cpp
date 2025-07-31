#include "algorithm.hpp"  
#include "input.hpp"
#include "output.hpp"
#include "density.hpp"

#include <iostream>

///
/// @brief Constructor for Algorithm.
///
Algorithm::Algorithm(Output& out) : out(out) {}

//----------------------------------------------------------------------
///
/// @brief Integrates the density of the input cube file.
///
void Algorithm::integrate_density(const Input& inp) {

    cube.read_density(inp.density_file_integration, false, "");

    cube.int_density();

    out.print_density(inp.density_file_integration, cube);
}
//----------------------------------------------------------------------
