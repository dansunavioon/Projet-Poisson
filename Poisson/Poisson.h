#ifndef POISSON_H
#define POISSON_H

#include <vector>
#include <SDL.h>

class Poisson
{
public:
    Poisson(float x, float y);

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;
    void applyBehaviors(const std::vector<Poisson>& poissons);

private:
    SDL_Point position;
    SDL_Point velocity;

    // Attribut pour déterminer si le poisson est indépendant
    bool independent;

    SDL_Point align(const std::vector<Poisson>& poissons);
    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);
};

#endif // POISSON_H