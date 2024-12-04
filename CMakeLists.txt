# Lier SDL2 à l'exécutable Projet_Poisson
target_link_libraries(Projet_Poisson ${SDL2_LIBRARIES})



cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher la bibliothèque SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source et déclarer l'exécutable
add_executable(Projet_Poisson main.cpp
        Poisson/Poisson.cpp
        Poisson/Poisson.h
        Personage/Personnage.cpp
        Personage/Personnage.h
        Personage/var_personnage.h
        Personage/var_personnage.cpp
        Fenetre/Fenetre.cpp
        Fenetre/Fenetre.h
)

# Inclure les fichiers d'en-tête SDL2
target_include_directories(Projet_Poisson PRIVATE ${SDL2_INCLUDE_DIRS})

# Lier la bibliothèque SDL2 à votre exécutable
target_link_libraries(Projet_Poisson PRIVATE ${SDL2_LIBRARIES})

