#ifndef SHARKBOSS_H
#define SHARKBOSS_H

#include <SDL2/SDL.h>
#include <vector>
#include "Poisson.h"

class SharkBoss {
public:
    SharkBoss(SDL_Renderer* renderer, float x, float y);
    ~SharkBoss();

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;

private:
    SDL_Renderer* renderer;
    SDL_Point position; // Position du requin
    SDL_Point velocity; // Vitesse
    float angle;        // Orientation du requin
    SDL_Color color;    // Couleur du requin

    void chaseFish(const std::vector<Poisson>& poissons);
    void limitSpeed();
};

#endif // SHARKBOSS_H