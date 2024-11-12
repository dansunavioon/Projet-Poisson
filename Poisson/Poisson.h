#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <vector>

class Poisson {
public:
    Poisson(float x, float y, SDL_Renderer* renderer);
    ~Poisson();

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;

private:
    SDL_Point position;
    SDL_Point velocity;
    int groupId;
    bool independent;
    SDL_Texture* poissonTexture;  // Texture de l'image du poisson

    void applyBehaviors(const std::vector<Poisson>& poissons);
    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);
};

#endif
