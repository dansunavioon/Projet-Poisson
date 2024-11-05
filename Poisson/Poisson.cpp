#include "Poisson.h"
#include <cmath>
#include <algorithm>

Poisson::Poisson(float x, float y) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
    velocity.x = (rand() % 3 - 1); // Vitesse initiale aléatoire
    velocity.y = (rand() % 3 - 1); // Vitesse initiale aléatoire
}

void Poisson::update(const std::vector<Poisson>& poissons) {
    applyBehaviors(poissons);
    position.x += velocity.x;
    position.y += velocity.y;

    // Wrap around screen
    if (position.x > 800) position.x = 0;
    else if (position.x < 0) position.x = 800;
    if (position.y > 600) position.y = 0;
    else if (position.y < 0) position.y = 600;
}

void Poisson::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void Poisson::applyBehaviors(const std::vector<Poisson>& poissons) {
    SDL_Point alignment = align(poissons);
    SDL_Point cohesion = cohesionBehavior(poissons);
    SDL_Point separation = separationBehavior(poissons);

    velocity.x += alignment.x + cohesion.x + separation.x;
    velocity.y += alignment.y + cohesion.y + separation.y;

    // Limiter la vitesse
    float speedLimit = 4.0f;
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > speedLimit) {
        velocity.x = (velocity.x / speed) * speedLimit;
        velocity.y = (velocity.y / speed) * speedLimit;
    }
}

SDL_Point Poisson::align(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 50.0f;

    for (const auto& other : poissons) {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius) {
            steering.x += other.velocity.x;
            steering.y += other.velocity.y;
            total++;
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit) {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}

SDL_Point Poisson::cohesionBehavior(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 50.0f;

    for (const auto& other : poissons) {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius) {
            steering.x += other.position.x;
            steering.y += other.position.y;
            total++;
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;

        steering.x -= position.x;
        steering.y -= position.y;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit) {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}

SDL_Point Poisson::separationBehavior(const std::vector<Poisson>& poissons) {
    SDL_Point steering = {0, 0};
    int total = 0;
    float perceptionRadius = 25.0f;

    for (const auto& other : poissons) {
        float distance = std::sqrt(std::pow(position.x - other.position.x, 2) + std::pow(position.y - other.position.y, 2));
        if (&other != this && distance < perceptionRadius) {
            float diff_x = position.x - other.position.x;
            float diff_y = position.y - other.position.y;
            steering.x += diff_x / distance;
            steering.y += diff_y / distance;
            total++;
        }
    }

    if (total > 0) {
        steering.x /= total;
        steering.y /= total;

        float speedLimit = 4.0f;
        float speed = std::sqrt(steering.x * steering.x + steering.y * steering.y);
        if (speed > speedLimit) {
            steering.x = (steering.x / speed) * speedLimit;
            steering.y = (steering.y / speed) * speedLimit;
        }
    }

    return steering;
}