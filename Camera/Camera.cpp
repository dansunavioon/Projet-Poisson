#include "Camera.h"

Camera::Camera(int x, int y) {
    // Initialisation de la caméra
    this->camera = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Camera::updateCamera(const SDL_Rect& personnageRect) {
    // Centrer la caméra sur le personnage
    this->camera = { personnageRect.x, personnageRect.y, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Empêcher la caméra de sortir des limites de la carte
    if (this->camera.x < 0) this->camera.x = 0;
    if (this->camera.y < 0) this->camera.y = 0;
    if (this->camera.x > MAP_WIDTH - this->camera.w) this->camera.x = MAP_WIDTH - camera.w;
    if (this->camera.y > MAP_HEIGHT - this->camera.h) this->camera.y = MAP_HEIGHT - camera.h;
}

int Camera::getX() {
    return this->camera.x;
}

int Camera::getY() {
    return this->camera.y;
}

