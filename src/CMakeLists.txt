# src/CMakeLists.txt

# Add all relevant C++ source files to the FretLab target
target_sources(FretLab
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/algorithm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/density_nanoparticle/integrals.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/density_nanoparticle/nanoparticle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/density_nanoparticle/density.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/input_output/output.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/input_output/input.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/string_manipulation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/timer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/parameters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Make headers in src/ and subfolders accessible
target_include_directories(FretLab
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/input_output
        ${CMAKE_CURRENT_SOURCE_DIR}/tools
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithm
        ${CMAKE_CURRENT_SOURCE_DIR}/density_nanoparticle
)

