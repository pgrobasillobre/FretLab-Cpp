#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include <chrono>

/// @brief Handles performance timing for labeled code sections.
/// 
/// This class allows measuring the elapsed wall-clock time of specific
/// named sections (e.g., "total", "step1", etc.) using high-resolution clocks.
/// Timers must be initialized before they can be started.
class Timer {
public:
    /// @brief Initializes the default timers.
    ///
    /// This function prepares internal data structures.
    void initialize();

    /// @brief Starts the timer for a specific task.
    ///
    /// Records the current high-resolution time for the given timer name.
    /// If the timer name is not initialized, a runtime error is thrown.
    ///
    /// @param name Name of the timer (e.g., "total", "step1").
    void start(const std::string& name);
    
    /// @brief Finishes the timer for a specific task.
    ///
    /// Records the end time for the specified timer. If the timer was not
    /// initialized or started, a runtime error is thrown.
    ///
    /// @param name Name of the timer to finish.
    void finish(const std::string& name);

    /// @brief Finalizes all timers and prints a summary.
    ///
    /// Prints elapsed time information for each timer to the provided output stream.
    /// Also includes date/time and program termination message.
    ///
    /// @param out Output stream to write the report to (default is std::cout).
    void conclude(std::ostream& out = std::cout);

    // TODO: Add finish() and conclude() methods for reporting.

private:
    /// @brief Stores time data for each timer.
    struct TimeData {
        std::chrono::high_resolution_clock::time_point start_time; ///< Start timestamp
        std::chrono::high_resolution_clock::time_point end_time;   ///< End timestamp
        bool started = false;   ///< True if the timer has been started
        bool finished = false;  ///< True if the timer has been finished
    };

    /// @brief Map of timer names to their timing data.
    std::unordered_map<std::string, TimeData> timers;
};

#endif 
