cmake_minimum_required(VERSION 3.29)
project(ProjektowanieEfektywnychAlgorytmow)

set(CMAKE_CXX_STANDARD 20)

add_executable(brutalne BruteForce.cpp Graph.cpp Graph.h
        Configuration.cpp
        Configuration.h)
add_executable(sasiedzkie NearestNeighbor.cpp Graph.cpp Graph.h
        Configuration.cpp
        Configuration.h)
add_executable(generatorproblemow generatorproblemow.cpp Graph.cpp Graph.h
        Configuration.cpp
        Configuration.h)
add_executable(random random.cpp Graph.cpp Graph.h
        Configuration.cpp
        Configuration.h)
add_executable(brannbound branchnbound.cpp Graph.cpp Graph.h
        Configuration.cpp
        Configuration.h)