#include <exception>
#include <iostream>

#include "target.hpp"
#include "input.hpp"
#include "output.hpp"
#include "timer.hpp"
#include "algorithm.hpp"

//---------------------------------------------------------------------------
//                     ______          __  __          __
//                    / ____/_______  / /_/ /   ____ _/ /__
//                   / /_  / ___/ _ \/ __/ /   / __ '/__   /
//                  / __/ / /  /  __/ /_/ /___/ /_/ / /_/ /
//                 /_/   /_/   \___/\__/_____/\__,_/_.___/
//
//---------------------------------------------------------------------------
//
//                       Program by Pablo Grobas Illobre
//
//                         For any problem write to:
//                         pgrobasillobre@gmail.com
//
//---------------------------------------------------------------------------
/// @file main.cpp
/// @brief Main entry point of the FretLab program.
///
/// This function sets up and coordinates the key components:
/// - Parses command-line arguments
/// - Reads and checks input
/// - Dispatches the appropriate computational algorithm based on the target
/// - Manages performance timing
/// - Handles error reporting and output
int main(int argc, char *argv[])
{

    Output out;

    try
    {
        // Instantiate components
        Timer timer;
        Input inp;
        Target target;

        // Parse input arguments
        inp.get_arguments(argc, argv, out, target);
        timer.initialize();
        timer.start("total");

        // Open output file. Check input file existence and extension
        out.open();
        inp.check_input_file(out);

        // Print banner, read input and print input info.
        out.print_banner();

        inp.read(target);
        inp.print_input_info(out, target);

        // Initialize algorithm instance with output and target references.
        Algorithm algorithm(out, target);

        switch (target.mode)
        {
        case TargetMode::IntegrateCube:
            algorithm.integrate_density(target);
            break;

        case TargetMode::Acceptor_Donor:
            algorithm.acceptor_donor(target);
            break;

        case TargetMode::Acceptor_NP:
            algorithm.acceptor_np(target);
            break;

        case TargetMode::Acceptor_NP_Donor:
            algorithm.acceptor_np_donor(target);
            break;

            case TargetMode::None:
        default:
            throw std::runtime_error("No valid calculation target specified in input.");
        }

        // Finalize timing and output
        timer.finish("total");
        timer.conclude(out);

        out.close();
    }
    catch (const std::exception &e)
    {
        out.stream() << " Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
