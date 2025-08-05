#include "timer.hpp"
#include "output.hpp"

#include <iomanip> 
#include <stdexcept>
#include <ctime>

//----------------------------------------------------------------------
///
/// @brief Initializes the timer module by setting up default timers.
///
/// Adds the "total" timer to the internal map. This is analogous to
/// initializing the `strings_timer` array in the original Fortran code.
void Timer::initialize() {
    timers["total"] = TimeData{};
}
//----------------------------------------------------------------------
///
/// @brief Starts timing for a specified named section.
///
/// If the timer has already been initialized, this function will record
/// the current time as the start time. If the timer has not been initialized,
/// a runtime exception will be thrown.
///
/// @param name The name of the timer to start (must already exist).
///
/// @throws std::runtime_error if the timer name is not found.
void Timer::start(const std::string& name) {
    auto it = timers.find(name);
    if (it != timers.end()) {
        it->second.start_time = std::chrono::high_resolution_clock::now();
        it->second.started = true;
    } else {
        throw std::runtime_error("Timer \"" + name + "\" is not initialized");
    }
}
//----------------------------------------------------------------------
///
/// @brief Finishes timing for a specified named section.
///
/// Records the current high-resolution end time. If the timer was
/// not initialized or not started, a runtime error is thrown.
///
/// @param name The name of the timer to finish.
/// @throws std::runtime_error if the timer name is not found or not started.
void Timer::finish(const std::string& name) {
    auto it = timers.find(name);
    if (it != timers.end() && it->second.started) {
        it->second.end_time = std::chrono::high_resolution_clock::now();
        it->second.finished = true;
    } else {
        throw std::runtime_error("Timer \"" + name + "\" was not started or initialized");
    }
}
//----------------------------------------------------------------------
///
/// @brief Prints a summary report of all timers to the provided output stream.
///
/// Outputs elapsed wall-clock time for each completed timer, along with
/// current date and time of program termination. Incomplete timers are noted.
///
/// @param out Output instance to print the report (default is std::cout).
void Timer::conclude(const Output& out) {
    using namespace std::chrono;

    const auto now = system_clock::now();
    const std::time_t end_time = system_clock::to_time_t(now);
    const std::tm* tm_ptr = std::localtime(&end_time);

    // Hardcoded total time only
    const std::string timer_name = "total";

    long long elapsed_seconds = 0;
    if (timers.count(timer_name) && timers[timer_name].started && timers[timer_name].finished) {
        elapsed_seconds = duration_cast<seconds>(
            timers[timer_name].end_time - timers[timer_name].start_time
        ).count();
    }

    int hours = static_cast<int>(elapsed_seconds / 3600);
    int minutes = static_cast<int>((elapsed_seconds % 3600) / 60);
    int seconds = static_cast<int>(elapsed_seconds % 60);

    // Header
    //out.stream() << " " << out.sticks << "\n\n";
    out.stream() << std::setw(28) << " " << "We should translate this Fortran code into C++.\n\n";
    out.stream() << std::setw(53) << " " << "-- P. Grobas Illobre\n\n";
    out.stream() << " "  << out.sticks << "\n\n";

    // Timing Info
    out.stream() << std::setw(42) << " " << "CPU Time:    "
               << std::setw(3) << hours << " h "
               << std::setw(2) << minutes << " min "
               << std::setw(2) << seconds << " sec\n";

    out.stream() << std::setw(42) << " " << "Elapsed Time:"
               << std::setw(3) << hours << " h "
               << std::setw(2) << minutes << " min "
               << std::setw(2) << seconds << " sec\n";

    out.stream()  << "\n " << out.sticks << "\n\n";

    // Date/Time Stamp
    out.stream() << std::setw(4) << " "
               << "Normal Termination of FretLab program in date "
               << std::put_time(tm_ptr, "%d/%m/%Y at %H:%M:%S") << "\n\n";

    out.stream() << " " << out.sticks << "\n";
}
//----------------------------------------------------------------------

