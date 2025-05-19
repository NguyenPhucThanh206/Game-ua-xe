#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    Obstacle(SDL_Renderer* renderer);
    ~Obstacle();

    void update();
    void render();
    void reset(); 
    SDL_Rect getRect(); 
    bool passed;
	bool isPassed();
	
	int getLane();
    int getY();
    void resetAtLane(int laneIndex);
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    SDL_Rect rect;
    int speed;
    int lane;
};

#endif

