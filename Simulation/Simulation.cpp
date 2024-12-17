#include "Simulation.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

Simulation::Simulation()
    : window(nullptr), renderer(nullptr), sharkBoss(nullptr), cameraPosition{0, 0}, timerID(0) {}

Simulation::~Simulation() {
    cleanUp();
}

bool Simulation::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "Erreur d'initialisation SDL : " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Simulation de poissons",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CAMERA_WIDTH, CAMERA_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        return false;
    }

    srand(static_cast<unsigned>(time(0)));
    initializePoissons();
    initializeSharkBoss();

    // Initialiser le timer
    const int updateInterval = 16; // ~60 FPS
    timerID = SDL_AddTimer(updateInterval, updateCallback, this);

    return true;
}

void Simulation::initializePoissons() {
    const int nombrePoissons = 100;
    for (int i = 0; i < nombrePoissons; ++i) {
        float randomX = static_cast<float>(rand() % MAP_WIDTH);
        float randomY = static_cast<float>(rand() % MAP_HEIGHT);
        bool independent = (rand() % 6 == 0);
        poissons.emplace_back(renderer, randomX, randomY, independent);
    }
}

void Simulation::initializeSharkBoss() {
    float sharkX = static_cast<float>(MAP_WIDTH / 2);
    float sharkY = static_cast<float>(MAP_HEIGHT / 2);
    sharkBoss = new SharkBoss(renderer, sharkX, sharkY);
}

void Simulation::run() {
    bool running = true;

    while (running) {
        handleEvents(running);
        update();  // Mise à jour des entités, y compris le requin
        render();  // Rendu de la scène
        SDL_Delay(14); // Pour limiter la vitesse d'exécution
    }
}

void Simulation::handleEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    const int cameraSpeed = 8;

    if (keystate[SDL_SCANCODE_UP]) cameraPosition.y -= cameraSpeed;
    if (keystate[SDL_SCANCODE_DOWN]) cameraPosition.y += cameraSpeed;
    if (keystate[SDL_SCANCODE_LEFT]) cameraPosition.x -= cameraSpeed;
    if (keystate[SDL_SCANCODE_RIGHT]) cameraPosition.x += cameraSpeed;

    cameraPosition.x = std::clamp(cameraPosition.x, 0, MAP_WIDTH - CAMERA_WIDTH);
    cameraPosition.y = std::clamp(cameraPosition.y, 0, MAP_HEIGHT - CAMERA_HEIGHT);
}

void Simulation::update() {
    {
        std::lock_guard<std::mutex> lock(poissonsMutex);
        for (auto& poisson : poissons) {
            poisson.respawnIfNeeded(MAP_WIDTH, MAP_HEIGHT);
            poisson.update(poissons);
        }
    }

    // Mettre à jour le requin (logique de chasse)
    if (sharkBoss) {
        sharkBoss->hunt(poissons);
    }
}

void Simulation::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu pour la mer
    SDL_RenderClear(renderer);

    {
        std::lock_guard<std::mutex> lock(poissonsMutex);
        for (const auto& poisson : poissons) {
            if (!poisson.isCaptured()) {
                poisson.draw(renderer, cameraPosition);
            }
        }
    }

    if (sharkBoss) {
        sharkBoss->draw(renderer, cameraPosition);
    }

    SDL_RenderPresent(renderer);
}

void Simulation::cleanUp() {
    if (timerID) {
        SDL_RemoveTimer(timerID);
    }

    delete sharkBoss;

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}

Uint32 Simulation::updateCallback(Uint32 interval, void* param) {
    auto* simulation = static_cast<Simulation*>(param);
    if (simulation) {
        simulation->update(); // Appel de la mise à jour
    }
    return interval;
}
