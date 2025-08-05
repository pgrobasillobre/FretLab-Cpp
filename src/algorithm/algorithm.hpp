#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "target.hpp"
#include "output.hpp"
#include "density.hpp"

///
/// @class Algorithm
/// @brief Handles algorithmic operations.
///
class Algorithm {
public:
    /// Constructor
    Algorithm(Output& out);

    ///
    /// @brief Integrates density of input cube file.
    /// 
    void integrate_density(const Target& target);

    ///
    /// @brief Compute electronic energy transfer rate between donor and acceptor.
    ///
    void acceptor_donor(const Target& target);

private: 
    Output& out;
    Density cube; 
};

#endif 

