#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>

// Dimensions de la carte
extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;

// Dimensions de la caméra
extern const int CAMERA_WIDTH;
extern const int CAMERA_HEIGHT;


class Poisson
{
public:
    Poisson(SDL_Renderer* renderer, float x, float y, bool independent);
    ~Poisson();

    bool isCaptured() const { return captured; }
    void setCaptured(bool state) { captured = state; }
    void setRespawnTime(Uint32 time) { respawnTime = time; }
    void respawnIfNeeded(int mapWidth, int mapHeight);

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer, int cameraX, int cameraY) const;

    SDL_Point getPosition() const { return position; }

private:
    void applyBehaviors(const std::vector<Poisson>& poissons);

    SDL_Point cohesionBehavior(const std::vector<Poisson>& poissons);
    SDL_Point separationBehavior(const std::vector<Poisson>& poissons);
    float angle;

    SDL_Point position;
    SDL_Point velocity;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool independent;
    int groupId;
    bool captured;
    Uint32 respawnTime;

    static constexpr int MAX_GROUPS = 20;
};

#endif // POISSON_H