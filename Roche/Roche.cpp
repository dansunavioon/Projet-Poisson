//
// Created by Florian BOISDRON on 05/11/2024.
//

#include "Roche.h"

Roche::Roche(SDL_Renderer *renderer, int height, int width, int x, int y) {
    this->renderer = renderer;
    this->height = height;
    this->width = width;
    this->x = x;
    this->y = y;

    this->display();
}


void Roche::display() const {
    const SDL_Rect obstacleRect = { this->x, this->y, this->width, this->height };
    SDL_RenderFillRect(this->renderer, &obstacleRect);
}
