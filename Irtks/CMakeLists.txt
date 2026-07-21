cmake_minimum_required(VERSION 3.16)
project(Pozo_Maldonado_Kevin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(examen02_omp main_omp.cpp blur_omp.cpp)
target_link_libraries(examen02_omp PRIVATE OpenMP::OpenMP_CXX SFML::Graphics SFML::Window SFML::System)

include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    add_executable(examen02_cuda main_cuda.cpp blur_cuda.cu)
    target_link_libraries(examen02_cuda PRIVATE SFML::Graphics SFML::Window SFML::System)
    set_target_properties(examen02_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
else()
    add_executable(examen02_cuda main_cuda.cpp blur_cuda_dummy.cpp)
    target_link_libraries(examen02_cuda PRIVATE SFML::Graphics SFML::Window SFML::System)
endif()
