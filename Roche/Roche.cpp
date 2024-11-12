#include "Roche.h"

Roche::Roche(SDL_Renderer* renderer, int height, int width, int x, int y)
    : renderer(renderer), height(height), width(width), x(x), y(y) {
}

void Roche::display() const {
    const SDL_Rect obstacleRect = { this->x, this->y, this->width, this->height };
    SDL_RenderFillRect(this->renderer, &obstacleRect);
}