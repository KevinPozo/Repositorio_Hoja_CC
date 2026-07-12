cmake_minimum_required(VERSION 3.20)
project(MpiPractice)

set(VCPKG_APPLOCAL_DEPS OFF)

find_package(fmt CONFIG REQUIRED)

set(MPI_ROOT "c:/oneApi/mpi/latest")
set(MPI_INCLUDE "${MPI_ROOT}/include")
set(MPI_LIB "${MPI_ROOT}/lib")

include_directories(${MPI_INCLUDE})

add_executable(mpi_practice main.cpp solver.cpp)
target_link_libraries(mpi_practice PRIVATE
    fmt::fmt
    ${MPI_LIB}/impi.lib
)
