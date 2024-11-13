#include "Poisson.h"
#include <cmath>
#include <cstdlib>
#include <iostream> // Pour le débogage

Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent): renderer(renderer), texture(nullptr), independent(independent)
{
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 - 1); // Vitesse initiale aléatoire
    velocity.y = (rand() % 3 - 1); // Vitesse initiale aléatoire

    // Réduit la probabilité d'indépendance à 25 % au lieu de 50 %
    independent = (rand() % 4 == 0);  // Chance de 25% pour être indépendant

    // Assigner un groupe aléatoire parmi un nombre défini de groupes
    groupId = rand() % MAX_GROUPS; // MAX_GROUPS défini comme le nombre maximum de groupes
}

void Poisson::update(const std::vector<Poisson>& poissons)
{
    applyBehaviors(poissons);

    // Si le poisson est indépendant, ajoute un léger mouvement aléatoire
    if (independent)
    {
        velocity.x += (rand() % 3 - 1); // Mouvement aléatoire léger
        velocity.y += (rand() % 3 - 1); // Mouvement aléatoire léger
    }

    position.x += velocity.x;
    position.y += velocity.y;

    // Wrap around screen
    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer, float a, float b)
{
    // Couleur rouge pour les poissons en groupe, blanc pour les indépendants
    SDL_SetRenderDrawColor(renderer, independent ? 255 : 255, independent ? 255 : 0, independent ? 255 : 0, 255);

    SDL_Rect rect;
    rect.x = position.x - 5; // Centrer le rectangle autour de position.x
    rect.y = position.y - 5; // Centrer le rectangle autour de position.y
    rect.w = 10;             // Largeur du rectangle
    rect.h = 10;             // Hauteur du rectangle

    SDL_Surface* tempSurface = SDL_LoadBMP("Image_Poisson/Poisson1.bmp");
    if (!tempSurface) {
        SDL_Log("Erreur lors du chargement de l'image roche.bmp: %s", SDL_GetError());
        return;
    }

    this->texture = SDL_CreateTextureFromSurface(this->renderer, tempSurface);
    SDL_FreeSurface(tempSurface); // Libérer la surface après la création de la texture

    if (!this->texture) {
        SDL_Log("Erreur lors de la création de la texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderCopy(this->renderer, this->texture, nullptr, &rect);

    SDL_DestroyTexture(this->texture);  // Libérer la texture après affichage

}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons)
{
    if (!independent) // Applique les comportements de groupe seulement si le poisson n'est pas indépendant
    {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        velocity.x += cohesion.x + separation.x;
        velocity.y += cohesion.y + separation.y;
    }

    // Limiter la vitesse
    float speedLimit = 3.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit)
    {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 50.0f;

    for (const auto& other : poissons)
    {
        // Applique la cohésion seulement entre poissons du même groupe
        if (&other != this && other.groupId == groupId)  // Vérifier si le poisson appartient au même groupe
        {
            float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
            if (distance < perceptionRadius)
            {
                steering.x += other.position.x;
                steering.y += other.position.y;
                total++;
            }
        }
    }

    if (total > 0)
    {
        steering.x /= total;
        steering.y /= total;

        steering.x -= position.x + 16;
        steering.y -= position.y + 16;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit)
        {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons)
{
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 60.0f;  // Augmente légèrement le rayon de perception pour éviter la superposition

    for (const auto& other : poissons)
    {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius)
        {
            float diff_x = position.x - other.position.x;
            float diff_y = position.y - other.position.y;

            // Inverser la distance pour renforcer l'éloignement quand ils sont trop proches
            steering.x += diff_x / (distance * distance + 0.1f); // Ajouter 0.1 pour éviter la division par zéro
            steering.y += diff_y / (distance * distance + 0.1f);

            total++;
        }
    }

    if (total > 0)
    {
        steering.x /= total;
        steering.y /= total;

        // Limite de vitesse pour le steering de séparation
        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit)
        {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}

