#include "algorithm.hpp"  
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
void Algorithm::integrate_density(const Target& target) {

    cube.read_density(target.density_file_integration, false, "");

    cube.int_density();

    out.print_density(target.density_file_integration, cube);
}
//----------------------------------------------------------------------
///
/// @brief Compute electronic energy transfer rate between donor and acceptor. 
///
void Algorithm::acceptor_donor(const Target& target) {

    //cube.read_density(target.density_file_integration, false, "");

    std::cout << "within algorithm acceptor donor" << std::endl;

    //out.print_density(target.density_file_integration, cube);
}
//----------------------------------------------------------------------
