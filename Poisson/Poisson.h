#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <vector>

class Poisson {
public:
    Poisson(SDL_Renderer* renderer, float x, float y);
    ~Poisson();

    void draw(SDL_Renderer* renderer, SDL_Rect camera);
    void update(std::vector<Poisson>& poissons);

    float getX() const { return x; }
    float getY() const { return y; }

private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    float x, y;
};

#endif