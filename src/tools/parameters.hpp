#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>

// debugpgi
/// @brief Defines physical constants and header strings used across the application.

/// @namespace Parameters
/// @brief Namespace containing global constants and header labels.
namespace Parameters {

    // Physical constants
    constexpr double pi = 3.141592653589793;
    constexpr double hbar = 1.054571817e-34;
    constexpr double ToBohr = 1.8897261254578281;  // Conversion from Angstrom to Bohr
    constexpr double ToAng = 1.0 / ToBohr;         // Conversion from Bohr to Angstrom

    constexpr double QMscrnFact = 0.2;             // Screening factor for Coulomb integrals

    // Constraint for reduce density file
    constexpr int ncellmax = 10000000;

    // Header strings (declared here, defined in parameters.cpp)
    extern const std::string acceptor_header;
    extern const std::string donor_header;

} // namespace Parameters

#endif // PARAMETERS_HPP

