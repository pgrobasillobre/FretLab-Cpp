#include "algorithm.hpp"  
#include "input.hpp"

#include <iostream>

/// @brief Constructor for Algorithm.
Algorithm::Algorithm() {} 

//----------------------------------------------------------------------
/// @brief Integrates the density of the input cube file.
void Algorithm::integrate_density(const Input& inp) {
    std::cout << "Integrating density from file: " 
              << inp.density_file_integration_input << std::endl;
}
//----------------------------------------------------------------------