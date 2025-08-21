#include "algorithm.hpp"
#include "output.hpp"
#include "target.hpp"
#include "density.hpp"
#include "parameters.hpp"
#include "integrals.hpp"
#include "nanoparticle.hpp"

#include <iostream>

//----------------------------------------------------------------------
///
/// @brief Constructor for Algorithm.
///
Algorithm::Algorithm(Output &out, Target &target) : out(out), target(target) {}

//----------------------------------------------------------------------
///
/// @brief Integrates the density of the input cube file.
///
void Algorithm::integrate_density(Target &target)
{

    cube.read_density(target, out, "Cube");

    cube.int_density();

    out.print_density(target, cube);
}
//----------------------------------------------------------------------
///
/// @brief Compute electronic energy transfer rate between donor and acceptor.
///
void Algorithm::acceptor_donor(Target &target)
{
    //
    //  Read input files
    //
    cube_acceptor.read_density(target, out, "Acceptor");

    cube_donor.read_density(target, out, "Donor");
    //
    //   Print acceptor / donor density characteristics
    //
    out.print_density(target, cube_acceptor, Parameters::acceptor_header);
                        
    out.print_density(target, cube_donor, Parameters::donor_header);

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
void Algorithm::acceptor_np(Target &target)
{
    //
    //  Read input files
    //
    cube_acceptor.read_density(target, out, "Acceptor");

    np.read_nanoparticle(target, out);
    //
    //  Print acceptor / donor density characteristics
    //
    out.print_nanoparticle(np);

    out.print_density(target, cube_acceptor, Parameters::acceptor_header);
    //
    //  Compute integrals
    //
    integrals.acceptor_np(target, cube_acceptor, np);
    //
    //  Print results
    //
    out.print_results_integrals(target, integrals);
}
//----------------------------------------------------------------------
///
/// @brief Compute electronic energy transfer rate between donor and acceptor mediated by nanoparticle.
///
void Algorithm::acceptor_np_donor(Target &target)
{
    //
    //  Read input files
    //
    cube_acceptor.read_density(target, out, "Acceptor");

    cube_donor.read_density(target, out, "Donor");

    np.read_nanoparticle(target, out);
    //
    //  Print acceptor / donor density characteristics
    //
    out.print_nanoparticle(np);

    out.print_density(target, cube_acceptor, Parameters::acceptor_header);

    out.print_density(target,cube_donor, Parameters::donor_header);
    //
    //  Compute integrals
    //
    integrals.acceptor_donor(target, cube_acceptor, cube_donor);

    integrals.acceptor_np(target, cube_acceptor, np);
    //
    //  Print results
    //
    out.print_results_integrals(target, integrals);
}
//----------------------------------------------------------------------