#include "Map.h"
#include "defs.h"
#include <SDL_image.h>

Map::Map(SDL_Renderer* renderer) {
    this->renderer = renderer;
    scrollY = 0;

    SDL_Surface* surface = IMG_Load("img/backgroun6.jpg");
    if (surface) {
        background = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        background = NULL;
    }
}

Map::~Map() {
    if (background != NULL)
        SDL_DestroyTexture(background);
}

void Map::update() {
    scrollY += MAP_SCROLL_SPEED;
    if (scrollY >= SCREEN_HEIGHT)
        scrollY = 0;
}

void Map::render() {
    SDL_Rect dst1 = { 0, -SCREEN_HEIGHT + scrollY, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect dst2 = { 0, scrollY, SCREEN_WIDTH, SCREEN_HEIGHT };

    if (background != NULL) {
        SDL_RenderCopy(renderer, background, NULL, &dst1);
        SDL_RenderCopy(renderer, background, NULL, &dst2);
    }
}

