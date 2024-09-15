#include "Fenetre.h"


/**
 * Constructeur
 * @param window
 * @param renderer
 * @param height
 * @param width
 */
Fenetre::Fenetre(SDL_Window* window, SDL_Renderer* renderer, const int height, const int width)
{
 this->window = window;
 this->renderer = renderer;
 this->height = height;
 this->width = width;
}


int Fenetre::display()
{
 // Initialisation
 SDL_Init(SDL_INIT_EVERYTHING);


 this->window = SDL_CreateWindow("Poisson", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
 this->renderer = SDL_CreateRenderer(this->window, -1, 0);


 SDL_SetRenderDrawColor(this->renderer, 0, 0, 150, 100);
 SDL_RenderClear(this->renderer);


// Test dessin carré
 SDL_Rect rect = {10, 20, 100, 100};
 SDL_Color color = {255, 0, 0, 255};

 SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
 SDL_RenderDrawRect(renderer, &rect);

 SDL_RenderPresent(this->renderer);

 SDL_Event events;

 // Boucle infinie qui empeche la destruction de la fenetre tant que la fenetre n'est pas fermée
 bool isOpen = true;

 while (isOpen)
 {
  while (SDL_PollEvent(&events))
  {
   switch (events.type)
   {
   case SDL_QUIT:
    isOpen = false;
    break;

   default: break;
   }
  }
 }

 SDL_DestroyRenderer(this->renderer);
 SDL_DestroyWindow(this->window);
 SDL_Quit();
 return EXIT_SUCCESS;

}
