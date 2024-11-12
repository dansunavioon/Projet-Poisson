#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <vector>

class Poisson {
public:
    Poisson(SDL_Renderer* renderer, float x, float y, bool independent);
    ~Poisson();
    void update();
    void draw() const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Point position;
    SDL_Point velocity;
    bool independent;
};

#endif
