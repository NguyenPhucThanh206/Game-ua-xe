#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
    Player(SDL_Renderer* renderer, int carIndex);
    ~Player();

    void handleEvent(SDL_Event& e);
    void update();
    void render();
    SDL_Rect getRect();

private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;      // v? tr� & k�ch thu?c xe
    int lane;           // l�n hi?n t?i (0�3)
};

#endif

