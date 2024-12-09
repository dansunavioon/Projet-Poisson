#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <mutex>
#include "Fenetre/Fenetre.h"
#include "Personage/Personnage.h"
#include "Poisson/Poisson.h"
#include "Personage/var_personnage.h"

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // Initialisation de SDL_image
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    const int largeurFenetre = 800;
    const int hauteurFenetre = 600;

    // Création de la fenêtre
    Fenetre fenetre("Simulation Poissons", largeurFenetre, hauteurFenetre);
    if (!fenetre.estValide()) {
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Création du personnage
    Personnage plongeur(fenetre.obtenirRenderer());

    // Création des poissons
    std::vector<Poisson> poissons;
    int nombrePoissons = 200;
    for (int i = 0; i < nombrePoissons; ++i) {
        float randomX = static_cast<float>(rand() % MAP_WIDTH);
        float randomY = static_cast<float>(rand() % MAP_HEIGHT);
        poissons.emplace_back(fenetre.obtenirRenderer(), randomX, randomY);
    }

    SDL_Event events;
    bool isRunning = true;
    Uint32 animationTimer = 0;
    const Uint32 animationDelay = 100;

    SDL_Rect camera = {0, 0, largeurFenetre, hauteurFenetre};

    while (isRunning) {
        while (SDL_PollEvent(&events)) {
            if (events.type == SDL_QUIT) {
                isRunning = false;
            } else {
                plongeur.handleInput(events);
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - animationTimer >= animationDelay) {
            plongeur.update();
            animationTimer = currentTime;
        }

        SDL_Rect personnageRect = {plongeur.getX(), plongeur.getY(), TAILLE_PLONGEUR, TAILLE_PLONGEUR};
        fenetre.updateCamera(personnageRect);

        fenetre.effacer();
        plongeur.render(fenetre.obtenirRenderer(), camera);

        for (auto& poisson : poissons) {
            poisson.update(poissons);
            poisson.draw(fenetre.obtenirRenderer(), camera);
        }

        fenetre.afficher();
    }

    IMG_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
