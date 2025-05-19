#include "Player.h"
#include "defs.h"
#include <SDL_image.h>

Player::Player(SDL_Renderer* renderer, int carIndex) {
    this->renderer = renderer;
    char filePath[30];
	if (carIndex == 0)
   		strcpy(filePath, "img/car1.png");
	else if (carIndex == 1)
    	strcpy(filePath, "img/car2.png");
	else
    	strcpy(filePath, "img/car3.png");
    	
	SDL_Surface* surface = IMG_Load(filePath);
	if (surface)
 	   	texture = SDL_CreateTextureFromSurface(renderer, surface);
	else {
   		texture = NULL;
    	SDL_Log("Can't load car image: %s", IMG_GetError());
	}
	SDL_FreeSurface(surface);
	
	
    rect.w = 80;
    rect.h = 80;
    lane = 1;  
    rect.x = ROAD_X + lane * LANE_WIDTH + 10;
    rect.y = SCREEN_HEIGHT - rect.h - 20;
}

Player::~Player() {
    if (texture != NULL)
        SDL_DestroyTexture(texture);
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
            if (lane > 0) lane--;
            break;
        case SDLK_RIGHT:
            if (lane < 3) lane++;
            break;
        case SDLK_UP:
            if (rect.y > 0)
                rect.y -= 20;
            break;
        case SDLK_DOWN:
            if (rect.y + rect.h < SCREEN_HEIGHT)
                rect.y += 20;
            break;
        }

        rect.x = ROAD_X + lane * LANE_WIDTH + 10;
    }
}

void Player::update(){
	
}

void Player::render() {
    if (texture != NULL)
        SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Rect Player::getRect() {
    return rect;
}
