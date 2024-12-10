#ifndef MAP_H
#define MAP_H

#include <SDL.h>

class Map {
private:
    SDL_Renderer* renderer;
    int width, height;

public:
    Map(SDL_Renderer* renderer, int width, int height);
    void render(const SDL_Rect& camera); // Affiche la portion visible de la carte selon la caméra
    void update(); // Met à jour les éléments de la carte
};

#endif // MAP_H
