#include "Poisson.h"
#include <cmath>
#include <cstdlib>
#include <iostream> // Pour le débogage

Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent)
    : renderer(renderer), independent(independent) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 - 1); // Vitesse initiale aléatoire
    velocity.y = (rand() % 3 - 1);

    // Charger l'image du poisson
    SDL_Surface* tempSurface = SDL_LoadBMP("Image_Poisson/Poisson1.bmp");
    if (tempSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture) {
            std::cerr << "Erreur de création de la texture : " << SDL_GetError() << std::endl;
        }
    } else {
        std::cerr << "Erreur de chargement de l'image poisson.bmp : " << SDL_GetError() << std::endl;
        texture = nullptr;
    }
}

Poisson::~Poisson() {
    SDL_DestroyTexture(texture);
}

void Poisson::update() {
    position.x += velocity.x;
    position.y += velocity.y;

    // Limiter les poissons dans la fenêtre avec retour de position
    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw() const {
    if (texture) {
        SDL_Rect renderQuad = {position.x, position.y, 32, 32}; // Ajuster la taille
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    } else {
        std::cerr << "Texture non chargée pour ce poisson" << std::endl;
    }
}
