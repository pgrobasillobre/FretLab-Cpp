#ifndef INTEGRALS_HPP
#define INTEGRALS_HPP

#include "target.hpp"
#include "density.hpp"
//#include "nanoparticle.hpp"

///
/// @class Integrals
/// @brief Define functions to compute integrals and store results.
///
class Integrals
{
public:
  // Computed values
  double coulomb_acceptor_donor = 0.0;
  double overlap_acceptor_donor = 0.0;
  double acceptor_nanoparticle  = 0.0;

  // Functions to compute integrals (no internal state dependencies)
  void acceptor_donor(const Target &target, const Density &cube_acceptor, const Density &cube_donor);
  //void acceptor_nanoparticle_donor(const Target &target, const Density &cube_acceptor, const Density &cube_donor, const Nanoparticle &np);
};

#endif // INTEGRALS_HPP
