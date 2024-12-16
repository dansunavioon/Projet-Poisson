#include "SharkBoss.h"
#include <cmath>
#include <iostream>

SharkBoss::SharkBoss(SDL_Renderer* renderer, float x, float y)
    : renderer(renderer), position{x, y}, velocity{0, 0}, texture(nullptr)
{
    // Création d'une texture pour le requin
    SDL_Surface* tempSurface = SDL_LoadBMP("SharkBoss.bmp");
    if (tempSurface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture)
        {
            std::cerr << "Erreur de création de la texture SharkBoss : " << SDL_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Erreur de chargement de l'image SharkBoss.bmp : " << SDL_GetError() << std::endl;
    }
}

SharkBoss::~SharkBoss()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
    }
}

void SharkBoss::update(const std::vector<Poisson>& poissons)
{
    chaseFish(poissons);

    // Mise à jour de la position
    position.x += velocity.x;
    position.y += velocity.y;

    // Limiter la vitesse
    float speedLimit = 5.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit)
    {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

void SharkBoss::draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const
{
    if (texture)
    {
        SDL_Rect renderQuad = {
            static_cast<int>(position.x - cameraPosition.x),
            static_cast<int>(position.y - cameraPosition.y),
            100, 100 // Dimensions du requin
        };

        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    }
}

void SharkBoss::chaseFish(const std::vector<Poisson>& poissons)
{
    SDL_Point target = {0, 0};
    int closestDistance = INT_MAX;

    for (const auto& poisson : poissons)
    {
        SDL_Point fishPosition = poisson.getPosition(); // Utilisation du getter
        int dx = fishPosition.x - static_cast<int>(position.x);
        int dy = fishPosition.y - static_cast<int>(position.y);
        int distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < closestDistance)
        {
            closestDistance = distanceSquared;
            target.x = fishPosition.x;
            target.y = fishPosition.y;
        }
    }

    // Ajuster la vélocité pour chasser le poisson cible
    if (closestDistance != INT_MAX)
    {
        velocity.x = target.x - position.x;
        velocity.y = target.y - position.y;
    }
}
