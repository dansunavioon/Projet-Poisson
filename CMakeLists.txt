cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher la bibliothèque SDL2
find_package(SDL2 REQUIRED)

# Chemin vers la bibliothèque SDL_image dans cmake-build-debug
set(SDL_IMAGE_PATH "C:/Users/Kylian/CLionProjects/Projet-Poisson/cmake-build-debug/SDL_image")

set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source et déclarer l'exécutable
add_executable(
        Projet_Poisson main.cpp
        Poisson/Poisson.cpp
        Poisson/Poisson.h
)

# Inclure les fichiers d'en-tête de SDL_image
include_directories(${SDL_IMAGE_PATH}/include)

target_include_directories(Projet_Poisson PRIVATE ${SDL2_INCLUDE_DIRS})

# Lier la bibliothèque SDL2 à votre exécutable
target_link_libraries(Projet_Poisson PRIVATE ${SDL2_LIBRARIES} ${SDL_IMAGE_PATH}/lib/libSDL2_image.a)






