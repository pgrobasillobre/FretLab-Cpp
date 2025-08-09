#include "algorithm.hpp"  
#include "output.hpp"
#include "target.hpp"
#include "density.hpp"
#include "parameters.hpp"
#include "integrals.hpp"

#include <iostream>

///
/// @brief Constructor for Algorithm.
///
Algorithm::Algorithm(Output& out, Target& target): out(out), target(target) {}


//----------------------------------------------------------------------
///
/// @brief Integrates the density of the input cube file.
///
void Algorithm::integrate_density(const Target& target) {

    cube.read_density(target, false, "Cube");

    cube.int_density();

    out.print_density(target.density_file_integration, cube);
}
//----------------------------------------------------------------------
///
/// @brief Compute electronic energy transfer rate between donor and acceptor. 
///
void Algorithm::acceptor_donor(const Target& target) {
//
//  Read input files
//
    cube_acceptor.read_density(target, false, "Acceptor");

    cube_donor.read_density(target, false, "Donor");
//
//   Print acceptor / donor density characteristics
//
    out.print_density(target.acceptor_density_file, cube_acceptor, Parameters::acceptor_header);

    out.print_density(target.donor_density_file, cube_donor, Parameters::donor_header);
//
//  Compute integrals
//
    integrals.acceptor_donor(target, cube_acceptor, cube_donor);
//
//  Print results
//
    out.print_results_integrals(target, integrals);
}
//----------------------------------------------------------------------
///
/// @brief Compute acceptor - nanoparticle coupling.
///
void Algorithm::acceptor_np(const Target &target)
{
    //
    //  Read input files
    //
    cube_acceptor.read_density(target, false, "Acceptor");
    //
    //   Print acceptor / donor density characteristics
    //
    out.print_density(target.acceptor_density_file, cube_acceptor, Parameters::acceptor_header);

    //
    //  Compute integrals
    //
    //integrals.acceptor_np(target, cube_acceptor, cube_np);
    //
    //  Print results
    //
    //out.print_results_integrals(target, integrals);
}
//----------------------------------------------------------------------
