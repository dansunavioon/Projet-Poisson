cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher la bibliothèque SDL2
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Projet_Poisson main.cpp)

# Inclure les fichiers d'en-tête SDL2
target_include_directories(Projet_Poisson PRIVATE ${SDL2_INCLUDE_DIRS})

# Lier la bibliothèque SDL2 à votre exécutable
target_link_libraries(Projet_Poisson PRIVATE ${SDL2_LIBRARIES})
