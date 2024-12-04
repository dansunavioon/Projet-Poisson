#ifndef FENETRE_H
#define FENETRE_H

#include <SDL.h>
#include <string>
#include <vector>
#include "../Personage/Personnage.h"
#include "../Poisson/Poisson.h"

class Fenetre
{
public:
    Fenetre(const std::string& titre, int largeur, int hauteur);
    ~Fenetre();

    bool estValide() const;
    SDL_Renderer* obtenirRenderer() const;

    void effacer();
    void afficher();

    void updateCamera(const SDL_Rect& personnageRect);
    int display();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect camera; // Caméra pour suivre le personnage
    bool valide;

    int height;
    int width;
};

#endif // FENETRE_H
