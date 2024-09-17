cmake_minimum_required(VERSION 3.20)
project(Projet_Poisson)

set(CMAKE_CXX_STANDARD 17)

# Trouver SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Ajouter les fichiers source et déclarer l'exécutable
add_executable(Projet_Poisson main.cpp Poisson/Poisson.cpp)

# Lier SDL2 à l'exécutable Projet_Poisson
target_link_libraries(Projet_Poisson ${SDL2_LIBRARIES})
