#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "target.hpp"
#include "output.hpp"
#include "density.hpp"
#include "integrals.hpp"

///
/// @class Algorithm
/// @brief Handles algorithmic operations.
///
class Algorithm {
public:
    /// Constructor
    Algorithm(Output& out, Target& target);

    ///
    /// @brief Integrates density of input cube file.
    /// 
    void integrate_density(const Target& target);

    ///
    /// @brief Compute electronic energy transfer rate between donor and acceptor.
    ///
    void acceptor_donor(const Target& target);

    ///
    /// @brief Compute acceptor - nanoparticle coupling.
    ///
    void acceptor_np(const Target &target);

private: 
    Output& out;
    Target& target;
    Density cube; 
    Density cube_acceptor;
    Density cube_donor;
    Integrals integrals;
};

#endif 

