#ifndef REQUIN_H
#define REQUIN_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "../Poisson/Poisson.h"
#include <climits>
#include <cmath>
#include <iostream>
#include "../Personage/var_personnage.h"


class Requin {
public:
    Requin(SDL_Renderer* renderer, float x, float y);
    ~Requin();

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer, int cameraX, int cameraY) const;
    void hunt(std::vector<Poisson>& poissons);

private:
    SDL_Renderer* renderer;
    SDL_Point position;
    SDL_Point velocity;
    SDL_Texture* texture;

    void chaseFish(const std::vector<Poisson>& poissons);
    float angle;
};



#endif //REQUIN_H
