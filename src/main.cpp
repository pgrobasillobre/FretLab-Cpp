#include <exception>
#include <iostream>

#include "input.hpp"
#include "output.hpp"
#include "timer.hpp"
//// #include "target.hpp"
//// #include "algorithm.hpp"

// #include <omp.h>  // Uncomment if/when using OpenMP

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
    try {
        // Instantiate components
        Input inp;
        Output out;
        Timer timer;
        ////Target target;
        ////Algorithm algorithm;

        // Parse input arguments
        inp.get_arguments(argc, argv, out);
        timer.initialize();
        timer.start("total");

        ////// Optional: set OpenMP thread count
        ////// omp_set_num_threads(parallel.n_threads_OMP);

        // Open output file and print banner
        out.open();
        out.print_banner();

        ////inp.check_input_file();
        ////inp.read();
        ////inp.print_input_info();

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
        timer.conclude(out.stream());

        out.close();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

