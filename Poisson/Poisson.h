#ifndef POISSON_H
#define POISSON_H

#include <SDL2/SDL.h>
#include <vector>

class Poisson {
public:
    Poisson(float x, float y);
    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;  // Ajout de 'const' ici

private:
    SDL_Point position;
    SDL_Point velocity;

    void applyBehaviors(const std::vector<Poisson>& poissons);
    SDL_Point align(const std::vector<Poisson>& poissons);
    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);  // Renommé
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);  // Renommé
};

#endif // POISSON_H