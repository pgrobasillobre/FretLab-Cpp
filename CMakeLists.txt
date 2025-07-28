# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project and enable C++ language
project(FretLabCPP)
enable_language(CXX)

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom CMake module paths for additional find scripts (like FindMKL.cmake, etc.)
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/math
    /usr/local/share/cmake/Modules
)

# ========================
# OPTIONS FOR CONFIGURATION
# ========================

# Allow toggling of optional features
option(ENABLE_AUTO_BLAS      "Enable CMake to autodetect BLAS"                  ON)
option(ENABLE_AUTO_LAPACK    "Enable CMake to autodetect LAPACK"                ON)
option(ENABLE_THREADED_MKL   "Enable OpenMP parallelization in MKL"             ON)
option(ENABLE_OMP            "Enable OpenMP parallelization"                    OFF)

# ========================
# COMPILER FLAGS BY VENDOR
# ========================

# GNU Compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops -Wall")
endif()

# Intel Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES Intel)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -w1 -xHost")

    if(ENABLE_OMP)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopenmp -parallel")
    endif()
endif()

# PGI/NVIDIA Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Minform=inform -Mstandard -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Mbounds -Mchkptr")
endif()

# ========================
# TESTING CONFIGURATION
# ========================
# Includes optional ConfigTesting.cmake if it exists
## include(ConfigTesting) # debugpgi

# Set language bindings and detect math libraries via a custom CMake module
set(BLAS_LANG "CXX")
set(LAPACK_LANG "CXX")
set(MKL_COMPILER_BINDINGS ${CMAKE_CXX_COMPILER_ID})
## include(ConfigMath) # debugpgi

# Save combined LAPACK/BLAS libraries into a variable
if(LAPACK_FOUND AND BLAS_FOUND)
   set(lapackblas_libraries ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES})
endif()

# ========================
# BUILD TARGET
# ========================

# Create the main executable target (placeholder for now)
add_executable(FretLab "")

# Add source directory that defines and populates the executable
add_subdirectory(src)

# ========================
# LINKING LIBRARIES
# ========================

# Link LAPACK, BLAS, and optionally OpenMP
if(ENABLE_OMP AND OpenMP_CXX_FOUND)
    target_link_libraries(FretLab
        PRIVATE
        ${LAPACK_LIBRARIES}
        ${BLAS_LIBRARIES}
        OpenMP::OpenMP_CXX
    )
else()
    target_link_libraries(FretLab
        PRIVATE
        ${LAPACK_LIBRARIES}
        ${BLAS_LIBRARIES}
    )
endif()

# ========================
# TEST FILES DEPLOYMENT
# ========================

# Copy the test folder into the build directory
file(COPY ${CMAKE_SOURCE_DIR}/tests DESTINATION ${CMAKE_BINARY_DIR})

# Copy the runtest script if it exists
if(EXISTS ${CMAKE_SOURCE_DIR}/submodules/runtest/runtest)
   file(COPY ${CMAKE_SOURCE_DIR}/submodules/runtest/runtest DESTINATION ${CMAKE_BINARY_DIR}/tests)
endif()

