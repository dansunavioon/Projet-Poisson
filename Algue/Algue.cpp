#include "Algue.h"
#include <SDL_log.h>

Algue::Algue(SDL_Renderer* renderer, int height, int width, int x, int y)
    : renderer(renderer), height(height), width(width), x(x), y(y), texture(nullptr) {
    this->display();
}

void Algue::display() {
    const SDL_Rect renderQuad = { this->x, this->y, this->width, this->height };

    SDL_Surface* tempSurface = SDL_LoadBMP("../cmake-build-debug/image_map/algue.bmp");
    if (!tempSurface) {
        SDL_Log("Erreur lors du chargement de l'image roche.bmp: %s", SDL_GetError());
        return;
    }

    this->texture = SDL_CreateTextureFromSurface(this->renderer, tempSurface);
    SDL_FreeSurface(tempSurface); // Libérer la surface après la création de la texture

    if (!this->texture) {
        SDL_Log("Erreur lors de la création de la texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderCopy(this->renderer, this->texture, nullptr, &renderQuad);

    SDL_DestroyTexture(this->texture);  // Libérer la texture après affichage
}
