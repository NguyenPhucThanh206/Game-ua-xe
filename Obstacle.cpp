#include "Obstacle.h"
#include "defs.h"
#include <SDL_image.h>
#include <cstdlib>

Obstacle::Obstacle(SDL_Renderer* renderer) {
    this->renderer = renderer;

    const char* obstacleImgs[] = {
        "img/obstacle1.png",
        "img/obstacle2.png",
        "img/obstacle3.png"
    };
    int index = rand() % 3;
    SDL_Surface* surface = IMG_Load(obstacleImgs[index]);
    if (surface != NULL)
        texture = SDL_CreateTextureFromSurface(renderer, surface);
    else
        texture = NULL;
    SDL_FreeSurface(surface);

    rect.w = 80;
    rect.h = 80;
    speed = 5;

    reset();
}

Obstacle::~Obstacle() {
    if (texture != NULL)
        SDL_DestroyTexture(texture);
}

void Obstacle::reset() {
    lane = rand() % 4;
    rect.x = ROAD_X + lane * LANE_WIDTH + 10;
    rect.y = -rect.h - (rand() % 100);
    passed = false;
}

void Obstacle::resetAtLane(int laneIndex) {
    lane = laneIndex;
    rect.x = ROAD_X + lane * LANE_WIDTH + 10;
    rect.y = -rect.h - (rand() % 100);
    passed = false;
}

void Obstacle::update() {
    rect.y += speed;
    if (!passed && rect.y > SCREEN_HEIGHT) {
        passed = true;
    }
}

void Obstacle::render() {
    if (texture != NULL)
        SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Rect Obstacle::getRect() {
    return rect;
}

bool Obstacle::isPassed() {
    return passed;
}

int Obstacle::getLane() {
    return lane;
}

int Obstacle::getY() {
    return rect.y;
}

