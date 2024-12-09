#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include "../Personage/var_personnage.h"
#include "../TypeDef.h"

class Camera {
    SDL_Rect camera;

    public:
        Camera(int x, int y);
        void updateCamera(const SDL_Rect& personnageRect); // Mise à jour de la camera
        int getX();
        int getY();
};



#endif //FENETRE_H
