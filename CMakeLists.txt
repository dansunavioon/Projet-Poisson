# Lier SDL2 à l'exécutable Projet_Poisson
target_link_libraries(Projet_Poisson ${SDL2_LIBRARIES})

cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher la bibliothèque SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_DIR}/include)
include_directories(${SDL2_IMAGE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)

# Chemins vers SDL2
set(SDL2_DIR "C:/SDL/SDL2")
set(SDL2_IMAGE_DIR "C:/SDL/SDL2_image")

link_directories(${SDL2_DIR}/lib)
link_directories(${SDL2_IMAGE_DIR}/lib)

find_library(SDL2_LIBRARY NAMES SDL2)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image)

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


# Lier la bibliothèque SDL2 à votre exécutable
target_link_libraries(Projet_Poisson PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)









