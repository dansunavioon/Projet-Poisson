#ifndef SHARKBOSS_H
#define SHARKBOSS_H

#include <SDL.h>
#include <vector>
#include "Poisson.h"

class SharkBoss {
public:
    SharkBoss(SDL_Renderer* renderer, float x, float y);
    ~SharkBoss();

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const; // Signature corrigée
    void hunt(std::vector<Poisson>& poissons);

private:
    SDL_Renderer* renderer;
    SDL_Point position; // Position (en pixels)
    SDL_Point velocity; // Vélocité
    SDL_Texture* texture; // Texture pour le requin

    void chaseFish(const std::vector<Poisson>& poissons);

    float angle;
};

#endif