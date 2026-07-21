cmake_minimum_required(VERSION 3.20)
project(Prueba02PozoKevinCuda)

enable_language(CXX CUDA)

set(CMAKE_CXX_FLAGS_DEBUG "-O3 -ffast-math -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
find_package(Freetype REQUIRED)

set(SOURCES 
    main.cpp 
    fractal-cuda.cu 
    palette.cpp
)

add_executable(prueba-mpi ${SOURCES})

target_include_directories(prueba-mpi PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

target_link_libraries(prueba-mpi PRIVATE
    fmt::fmt
    SFML::Graphics SFML::Window SFML::System
    Freetype::Freetype
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/Arial.ttf COPYONLY)
if(CMAKE_CONFIGURATION_TYPES)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Arial.ttf ${CMAKE_CURRENT_BINARY_DIR}/${config}/Arial.ttf COPYONLY)
    endforeach()
endif()
