#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include "../Personage/Personnage.h"
#include "../Poisson/Poisson.h"
#include "../Personage/var_personnage.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "../Requin/Requin.h"


class Map {
    int width, height;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;

    std::vector<Poisson> poissons;
    Requin* sharkBoss;

    public:
        Map(SDL_Renderer* renderer, int width, int height);
        ~Map();
        void render(int cameraX, int cameraY); // Affiche la portion visible de la carte selon la caméra
        void initPoissons(int nbPoissons);

        int getWidth() const;
        int getHeight() const;
};

#endif // MAP_H
