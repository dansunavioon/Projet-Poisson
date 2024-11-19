#include "Poisson.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Poisson::Poisson(SDL_Renderer* renderer, float x, float y, bool independent)
    : renderer(renderer), independent(independent) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);

    // Initialiser la vitesse avec une direction aléatoire fixe
    float angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
    velocity.x = static_cast<int>(cos(angle) * MAX_SPEED);
    velocity.y = static_cast<int>(sin(angle) * MAX_SPEED);

    SDL_Surface* tempSurface = SDL_LoadBMP("Image_Poisson/Poisson1.bmp");
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
    // Applique les comportements de groupe seulement si le poisson n'est pas indépendant
    if (!independent) {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        // Ajuster la direction sans changer la norme de vitesse
        velocity.x += cohesion.x * COHESION_FORCE + separation.x * SEPARATION_FORCE;
        velocity.y += cohesion.y * COHESION_FORCE + separation.y * SEPARATION_FORCE;

        // Normaliser la direction pour maintenir MAX_SPEED
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > 0) {
            velocity.x = static_cast<int>((velocity.x / speed) * MAX_SPEED);
            velocity.y = static_cast<int>((velocity.y / speed) * MAX_SPEED);
        }
    }

    // Mise à jour de la position en utilisant la vitesse normale
    position.x += velocity.x;
    position.y += velocity.y;

    // Wrap around screen
    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer) const {
    if (texture) {
        SDL_Rect renderQuad = {position.x, position.y, 32, 32};
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    } else {
        std::cerr << "Texture non chargée pour ce poisson" << std::endl;
    }
}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons) {
    // Comportement appliqué seulement si le poisson fait partie d'un groupe
    if (!independent) {
        SDL_Point cohesion = cohesionBehavior(poissons);
        SDL_Point separation = separationBehavior(poissons);

        // Applique les forces de cohésion et séparation sans excéder MAX_SPEED
        velocity.x += cohesion.x * COHESION_FORCE + separation.x * SEPARATION_FORCE;
        velocity.y += cohesion.y * COHESION_FORCE + separation.y * SEPARATION_FORCE;

        // Réduit la vitesse si elle dépasse la limite
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (speed > MAX_SPEED) {
            velocity.x = static_cast<int>((velocity.x / speed) * MAX_SPEED);
            velocity.y = static_cast<int>((velocity.y / speed) * MAX_SPEED);
        }
    }
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 50.0f;

    for (const auto& other : poissons) {
        if (&other != this && other.groupId == groupId) {
            float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
            if (distance < perceptionRadius) {
                steering.x += other.position.x;
                steering.y += other.position.y;
                total++;
            }
        }
    }

    if (total > 0) {
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
    float perceptionRadius = 60.0f;

    for (const auto& other : poissons) {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius) {
            float diff_x = position.x - other.position.x;
            float diff_y = position.y - other.position.y;
            steering.x += diff_x / (distance * distance + 0.1f);
            steering.y += diff_y / (distance * distance + 0.1f);
            total++;
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;
    }

    return steering;
}
