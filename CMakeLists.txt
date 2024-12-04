cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher les bibliothèques SDL2 et SDL2_mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Définir la norme C++
set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source
add_executable(Projet_Poisson
        main.cpp
        poisson/poisson.cpp
        poisson/poisson.h
        fenetre/fenetre.cpp
        fenetre/fenetre.h
        Personage/Personnage.cpp
        Personage/Personnage.h
        Personage/var_personnage.h
        Personage/var_personnage.cpp
)

# Inclure les répertoires d'en-tête pour SDL2 et SDL2_mixer
target_include_directories(Projet_Poisson PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# Lier SDL2 et SDL2_mixer à l'exécutable
target_link_libraries(Projet_Poisson ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})