#include "Poisson.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent)
    : renderer(renderer), independent(independent), angle(0.0) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);
    velocity.y = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);

    SDL_Surface* tempSurface = SDL_LoadBMP("Image_Poisson/Poisson12.bmp");
    if (tempSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture) {
            std::cerr << "Erreur de création de la texture : " << SDL_GetError() << std::endl;
        }
    } else {
        std::cerr << "Erreur de chargement de l'image poisson.bmp : " << SDL_GetError() << std::endl;
        texture = nullptr;
    }

    groupId = rand() % MAX_GROUPS;
}

Poisson::~Poisson() {
    SDL_DestroyTexture(texture);
}

void Poisson::update(const std::vector<Poisson>& poissons) {
    applyBehaviors(poissons);

    position.x += velocity.x;
    position.y += velocity.y;

    angle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    // Corriger la position si elle dépasse les limites de la carte
    if (position.x >= MAP_WIDTH) position.x = 0;
    else if (position.x < 0) position.x = MAP_WIDTH;

    if (position.y >= MAP_HEIGHT) position.y = 0;
    else if (position.y < 0) position.y = MAP_HEIGHT;
}

void Poisson::draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const
{
    if (texture)
    {
        SDL_Rect renderQuad = {position.x - cameraPosition.x,position.y - cameraPosition.y,32,32}; // Ajustez la taille de l'image si nécessaire
        SDL_Point center = {16, 16}; // Point central pour la rotation

        // Déterminer si l'image doit être retournée horizontalement
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        // Retourner l'image si la vélocité x est négative (le poisson va vers la gauche)
        if (velocity.x < 0)
        {
            flip = SDL_FLIP_HORIZONTAL; // Retourner l'image horizontalement
            flip = SDL_FLIP_VERTICAL;
        }

        // Dessiner l'image avec l'angle de rotation et l'éventuel retournement
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, angle, &center, flip);
    }
}

// Comportements collectifs
void Poisson::applyBehaviors(const std::vector<Poisson>& poissons) {
    if (!independent)
    {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        velocity.x += cohesion.x + separation.x;
        velocity.y += cohesion.y + separation.y;
    }

    // Limiter la vitesse
    float speedLimit = 2.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit)
    {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 40.0f;

    for (const auto& other : poissons)
    {
        if (&other != this && other.groupId == groupId)
        {
            float distance = std::sqrt(std::pow(position.x - other.position.x, 3) + std::pow(position.y - other.position.y, 3));
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

        steering.x -= position.x;
        steering.y -= position.y;
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 30.0f;

    for (const auto& other : poissons)
    {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius)
        {
            steering.x += position.x - other.position.x;
            steering.y += position.y - other.position.y;
            total++;
        }
    }

    if (total > 0)
    {
        steering.x /= total;
        steering.y /= total;
    }

    return steering;
}
