cmake_minimum_required(VERSION 3.20)
project(Prueba01PozoKevin)

enable_language(CXX CUDA)

set(CMAKE_CXX_FLAGS_DEBUG "-O3 -ffast-math -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenMP REQUIRED)

set(MPI_ROOT "c:/oneApi/mpi/latest")
set(MPI_INCLUDE "${MPI_ROOT}/include")
set(MPI_LIB "${MPI_ROOT}/lib")

include_directories(${MPI_INCLUDE})

set(SOURCES 
    main.cpp 
    fractal-mpi.cpp 
    fractal-serial.cpp 
    fractal-omp.cpp 
    fractal-simd.cpp 
    fractal-cuda.cu 
    palette.cpp
)

add_executable(prueba-mpi ${SOURCES})

if(MSVC)
    set_source_files_properties(fractal-simd.cpp PROPERTIES COMPILE_OPTIONS "/arch:AVX2")
else()
    set_source_files_properties(fractal-simd.cpp PROPERTIES COMPILE_OPTIONS "-mavx2")
endif()

target_link_libraries(prueba-mpi PRIVATE OpenMP::OpenMP_CXX)
target_compile_definitions(prueba-mpi PRIVATE OPENMP_AVAILABLE)

target_include_directories(prueba-mpi PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

target_link_libraries(prueba-mpi PRIVATE
    fmt::fmt
    SFML::Graphics SFML::Window SFML::System
    Freetype::Freetype
    ${MPI_LIB}/impi.lib
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/Arial.ttf COPYONLY)
if(CMAKE_CONFIGURATION_TYPES)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/${config}/Arial.ttf COPYONLY)
    endforeach()
endif()
