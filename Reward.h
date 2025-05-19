#ifndef REWARD_H
#define REWARD_H

#include <SDL.h>

class Reward {
public:
    Reward(SDL_Renderer* renderer);
    ~Reward();

    void setPosition(int x, int y);
    void update();
    void render();
    SDL_Rect getRect();
    bool isCollected() const;
    void collect();
    bool isOutOfScreen() const;

private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    bool collected;
};

#endif

