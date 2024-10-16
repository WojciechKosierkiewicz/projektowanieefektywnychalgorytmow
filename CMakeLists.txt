cmake_minimum_required(VERSION 3.29)
project(ProjektowanieEfektywnychAlgorytmow)

set(CMAKE_CXX_STANDARD 20)

add_executable(brutalne BruteForce.cpp)
add_executable(sasiedzkie NearestNeighbor.cpp)
add_executable(generatorproblemow generatorproblemow.cpp)