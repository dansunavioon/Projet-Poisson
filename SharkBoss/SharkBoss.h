#ifndef SHARKBOSS_H
#define SHARKBOSS_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Poisson.h"
#include <climits>

class SharkBoss {
public:
    SharkBoss(SDL_Renderer* renderer, float x, float y);
    ~SharkBoss();

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const;
    void hunt(std::vector<Poisson>& poissons);

private:
    SDL_Renderer* renderer;
    SDL_Point position;
    SDL_Point velocity;
    SDL_Texture* texture;

    void chaseFish(const std::vector<Poisson>& poissons);
    float angle;
};

#endif