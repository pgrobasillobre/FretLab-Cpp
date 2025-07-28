#include <exception>
#include <iostream>

#include "input.hpp"
#include "output.hpp"
#include "timer.hpp"
//// #include "target.hpp"
//// #include "algorithm.hpp"

// #include <omp.h> 

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

int main(int argc, char* argv[]) {

    Output out;

    try {
        // Instantiate components
        Input inp;
        Timer timer;
        ////Algorithm algorithm;

        // Parse input arguments
        inp.get_arguments(argc, argv, out);
        timer.initialize();
        timer.start("total");

        ////// Optional: set OpenMP thread count
        ////// omp_set_num_threads(parallel.n_threads_OMP);

        // Open output file. Check input file existence and extension
        out.open();
        inp.check_input_file(out);

        // Print banner, read input and print input info.
        out.print_banner();

        inp.read();
        inp.print_input_info(out);
        
        switch (inp.target_mode) {
            case TargetMode::IntegrateCube:
            //algorithm.integrate_density();
            std::cout << " Inside HERE!!! "  << std::endl;
            break;

            case TargetMode::None:
            default:
                throw std::runtime_error("No valid calculation target specified in input.");
        }

        //// debugpgi -> start with integrate density
        ////// Main computation switch
        ////if (target.name == "integrate_density") {
        ////    algorithm.integrate_density();
        ////}
        ////// Uncomment and implement later:
        ////// else if (target.name == "aceptor_donor") {
        //////     algorithm.eet_aceptor_donor();
        ////// }
        ////// else if (target.name == "aceptor_np") {
        //////     algorithm.aceptor_np_interaction();
        ////// }
        ////// else if (target.name == "aceptor_np_donor") {
        //////     algorithm.aceptor_np_donor();
        ////// }

        // Finalize timing and output
        timer.finish("total");
        timer.conclude(out);

        out.close();

    } catch (const std::exception& e) {
        out.stream() << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

