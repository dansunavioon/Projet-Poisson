#include "Poisson.h"
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>

Poisson::Poisson(SDL_Renderer* renderer, float x, float y)
    : renderer(renderer), x(x), y(y) {
    texture = IMG_LoadTexture(renderer, "images/poisson.png");
}

Poisson::~Poisson() {
    if (texture) SDL_DestroyTexture(texture);
}

void Poisson::update(std::vector<Poisson>& poissons) {
    x += (rand() % 3 - 1);  // Simulation de mouvement aléatoire
    y += (rand() % 3 - 1);

    if (x < 0) x = 0;
    if (y < 0) y = 0;
}

void Poisson::draw(SDL_Renderer* renderer, SDL_Rect camera) {
    SDL_Rect destRect = {static_cast<int>(x) - camera.x, static_cast<int>(y) - camera.y, 30, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}


void Poisson::draw(SDL_Renderer* renderer, const SDL_Rect& camera) const {
    SDL_Rect dest = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};
    SDL_RenderCopy(renderer, sprite, nullptr, &dest);
}
