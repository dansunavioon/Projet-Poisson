cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher la bibliothèque SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(Projet_Poisson main.cpp
        Poisson/Poisson.cpp
        Poisson/Poisson.h
        Map/Map.cpp
        Map/Map.h
        typeDef.h
        Roche/Roche.cpp
        Roche/Roche.h)

# Inclure les fichiers d'en-tête SDL2
target_include_directories(Projet_Poisson PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Lier la bibliothèque SDL2 à votre exécutable
target_link_libraries(Projet_Poisson PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
