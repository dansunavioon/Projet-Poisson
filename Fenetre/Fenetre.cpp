#include "Fenetre.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

Fenetre::Fenetre(const std::string& titre, int largeur, int hauteur)
    : window(nullptr), renderer(nullptr), valide(false), height(hauteur), width(largeur) {
    window = SDL_CreateWindow(titre.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largeur, hauteur, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        window = nullptr;
        return;
    }

    camera = {0, 0, largeur, hauteur};
    valide = true;
}

Fenetre::~Fenetre() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
}

bool Fenetre::estValide() const {
    return valide;
}

SDL_Renderer* Fenetre::obtenirRenderer() const {
    return renderer;
}

void Fenetre::effacer() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Fenetre::afficher() {
    SDL_RenderPresent(renderer);
}

void Fenetre::updateCamera(const SDL_Rect& personnageRect) {
    camera.x = personnageRect.x + personnageRect.w / 2 - camera.w / 2;
    camera.y = personnageRect.y + personnageRect.h / 2 - camera.h / 2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
}