#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <vector>

class Poisson
{
public:
    Poisson(float x, float y);
    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer) const;

private:
    SDL_Point position;
    SDL_Point velocity;
    bool independent;
    int groupId;  // Ajout de l'ID de groupe pour chaque poisson

    void applyBehaviors(const std::vector<Poisson>& poissons);
    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);

    static const int MAX_GROUPS = 5; // Définir le nombre de groupes max (ajustez selon vos besoins)
};

#endif // POISSON_H
