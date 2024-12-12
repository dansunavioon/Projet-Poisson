#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "../TypeDef.h"
#include <cstdlib>
#include <vector>

#include "../Algue/Algue.h"
#include "../Roche/Roche.h"
#include "../Personage/Personnage.h"
#include "../Personage/var_personnage.h"

class Map {

    SDL_Window* window;
    SDL_Renderer* renderer;
    int height;
    int width;

    void genererRoches();
    void genererAlgues();
    void genererFond();

    public:
        Map(SDL_Window* window, SDL_Renderer* renderer, int height, int width);
        int generer();
};



#endif //MAP_H
