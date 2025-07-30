#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "input.hpp"
#include "output.hpp"
#include "density.hpp"


/// @brief Handles algorithmic operations.
class Algorithm {
public:
    /// Constructor
    Algorithm();

    /// @brief Integrates density of input cube file.
    void integrate_density(const Input& inp);

    Input inp;
    Output out;
    Density cube; 
};

#endif 

