#ifndef POISSON_H
#define POISSON_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

// Constantes
static const int MAP_WIDTH = 3000;
static const int MAP_HEIGHT = 2000;
static const int CAMERA_WIDTH = 800;
static const int CAMERA_HEIGHT = 600;

class Poisson {
public:
    Poisson(SDL_Renderer* renderer, float x, float y, bool independent);
    ~Poisson();

    bool isCaptured() const { return captured; }
    void setCaptured(bool state) { captured = state; }
    void setRespawnTime(Uint32 time) { respawnTime = time; }
    void respawnIfNeeded(int mapWidth, int mapHeight);

    void update(const std::vector<Poisson>& poissons);
    void draw(SDL_Renderer* renderer, const SDL_Point& cameraPosition) const;

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

#endif