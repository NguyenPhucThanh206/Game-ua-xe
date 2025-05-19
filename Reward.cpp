#include "Reward.h"
#include "defs.h"
#include <SDL_image.h>

Reward::Reward(SDL_Renderer* renderer) {
    this->renderer = renderer;
    collected = false;

    SDL_Surface* surface = IMG_Load("img/stars.png");
    if (surface != NULL) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        texture = NULL;
        SDL_Log("Can't load the stars': %s", IMG_GetError());
    }

    rect.w = 40;
    rect.h = 40;
    rect.x = 0;
    rect.y = -rect.h;
}

Reward::~Reward() {
    if (texture != NULL)
        SDL_DestroyTexture(texture);
}

void Reward::setPosition(int x, int y) {
    rect.x = x + (LANE_WIDTH - rect.w) / 2;  // can gi?a làn
    rect.y = y;
}

void Reward::update() {
    if (!collected)
        rect.y += MAP_SCROLL_SPEED;
}

void Reward::render() {
    if (!collected && texture != NULL) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

SDL_Rect Reward::getRect() {
    return rect;
}

bool Reward::isCollected() const {
    return collected;
}

void Reward::collect() {
    collected = true;
}

bool Reward::isOutOfScreen() const {
    return rect.y > SCREEN_HEIGHT;
}

