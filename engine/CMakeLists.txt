cmake_minimum_required(VERSION 3.16)

project(FEAEngine
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED)

add_executable(fea_engine
    src/main.cpp
    src/Node.cpp
    src/Material.cpp
    src/Model.cpp
)

target_include_directories(fea_engine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(fea_engine
    PRIVATE
    Eigen3::Eigen
)