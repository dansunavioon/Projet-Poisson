#include "Poisson.h"
#include <SDL_image.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent): renderer(renderer), independent(independent), angle(0.0)
{
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1); // Vitesse initiale aléatoire
    velocity.y = (rand() % 3 + 1) * (rand() % 2 == 0 ? 1 : -1);

    // Charger l'image du poisson avec SDL_image
    SDL_Surface* tempSurface = IMG_Load("Image_Poisson/Poisson12.png");
    if (tempSurface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!texture)
        {
            std::cerr << "Erreur de création de la texture : " << SDL_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Erreur de chargement de l'image Poisson12.png : " << IMG_GetError() << std::endl;
        texture = nullptr;
    }

    groupId = rand() % MAX_GROUPS;
}

Poisson::~Poisson()
{
    SDL_DestroyTexture(texture);
}

void Poisson::update(const std::vector<Poisson>& poissons)
{
    applyBehaviors(poissons);

    position.x += velocity.x;
    position.y += velocity.y;

    angle = std::atan2(velocity.y, velocity.x) * 180 / M_PI;

    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;

    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer) const
{
    if (texture)
    {
        SDL_Rect renderQuad = {position.x, position.y, 32, 32};
        SDL_Point center = {16, 16};
        SDL_RendererFlip flip = (velocity.x < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, angle, &center, flip);
    }
}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons)
{
    if (!independent)
    {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        velocity.x += cohesion.x + separation.x;
        velocity.y += cohesion.y + separation.y;
    }

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
        if (&other != this && other.groupId == groupId)
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
        steering.x -= position.x;
        steering.y -= position.y;
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons)
{
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
