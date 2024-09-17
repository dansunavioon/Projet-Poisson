#ifndef PROJET_POISSON_PERSONNAGE_H
#define PROJET_POISSON_PERSONNAGE_H


class Personnage {
private:
    // coordonnées
    int x, y;

    // vitesse de deplacement
    int vitesse;

    // frame courante
    int frame;

    // status d'animations
    int status;

public:
    Personnage();
    void handle_events();

    // montrer le plongeur
    void affichage();

    // Découpe les 4 sprites
    void set_clip();
};


#endif //PROJET_POISSON_PERSONNAGE_H
