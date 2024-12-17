#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL.h>
#include <vector>
#include <mutex>
#include "Poisson.h"
#include "SharkBoss.h"

class Simulation {
public:
    Simulation();
    ~Simulation();

    bool initialize();
    void run();
    void cleanUp();

private:
    void handleEvents(bool& running);
    void update();
    void render();

    // Gestion des poissons et du requin
    void initializePoissons();
    void initializeSharkBoss();

    // Membres SDL
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Éléments de la simulation
    std::vector<Poisson> poissons;
    std::mutex poissonsMutex;
    SharkBoss* sharkBoss = nullptr;

    // Caméra
    SDL_Point cameraPosition;

    // Constantes
    const int MAP_WIDTH = 3000;
    const int MAP_HEIGHT = 2000;
    const int CAMERA_WIDTH = 800;
    const int CAMERA_HEIGHT = 600;

    // Timer
    SDL_TimerID timerID;
    static Uint32 updateCallback(Uint32 interval, void* param);
};

#endif // SIMULATION_H
