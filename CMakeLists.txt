cmake_minimum_required(VERSION 3.25)
project(Projet_Poisson)

# Rechercher les bibliothèques SDL2 et SDL2_mixer
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer  REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# Définir la norme C++
set(CMAKE_CXX_STANDARD 17)

# Ajouter les fichiers source
add_executable(Projet_Poisson
        main.cpp
        Poisson/Poisson.cpp
        Poisson/Poisson.h
        Fenetre/Fenetre.cpp
        Fenetre/Fenetre.h
        Personage/Personnage.cpp
        Personage/Personnage.h
        Personage/var_personnage.h
        Personage/var_personnage.cpp
        Map/Map.cpp
        Map/Map.h
)

# Lier SDL2 et SDL2_mixer à l'exécutable
target_link_libraries(Projet_Poisson ${SDL2_LIBRARIES} SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)

add_custom_command(
        TARGET Projet_Poisson POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/ressource/*" "${CMAKE_BINARY_DIR}"
        COMMENT "Copie du répertoire 'ressource' dans le répertoire de build."
)