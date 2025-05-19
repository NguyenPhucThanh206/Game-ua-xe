#ifndef MAP_H
#define MAP_H

#include <SDL.h>

class Map {
public:
    Map(SDL_Renderer* renderer);
    ~Map();
    void update();
    void render();

private:
    SDL_Texture* background;
    SDL_Renderer* renderer;
    int scrollY;
};

#endif

