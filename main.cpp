#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "Map.h"
#include "Player.h"
#include "Obstacle.h"
#include <vector>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include "Reward.h"
#include "Menu.h"
#include <SDL_mixer.h>

bool canSpawnInLane(int laneIndex, const std::vector<Obstacle*>& obstacles, int minY = 200) {
    for (size_t i = 0; i < obstacles.size(); i++) {
        if (obstacles[i]->getLane() == laneIndex && obstacles[i]->getY() < minY) {
            return false;
        }
    }
    return true;
}

void shuffleLanes(int lanes[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = lanes[i];
        lanes[i] = lanes[j];
        lanes[j] = temp;
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Can't create SDL: %s", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    	SDL_Log("SDL_mixer init error: %s", Mix_GetError());
    	return 1;
	}

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("Can't create SDL_image': %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_Log("Can't create SDL_ttf': %s", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Game Ðua Xe",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Can't create the window': %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Can't create renderer': %s", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    
    Mix_Music* menuMusic = Mix_LoadMUS("img/menu_bgm.mp3");
	Mix_Music* gameMusic = Mix_LoadMUS("img/game_bgm.mp3");

	if (!menuMusic || !gameMusic) {
    	SDL_Log("Audio load error: %s", Mix_GetError());
	}
	Mix_PlayMusic(menuMusic, -1); 
	
	Menu menu(renderer);

	int menuChoice = menu.showMainMenu();
	while (menuChoice == MENU_OPTIONS) {
    	menu.showCarSelectMenu();
    	menuChoice = menu.showMainMenu();
	}

	if (menuChoice == MENU_EXIT) {
   		SDL_DestroyRenderer(renderer);
   	 	SDL_DestroyWindow(window);
    	IMG_Quit();
    	TTF_Quit();
    	SDL_Quit();
    	return 0;
	}	
	
	Mix_HaltMusic();
	Mix_PlayMusic(gameMusic, -1);
	int selectedCar = menu.getSelectedCar();
	
	Reward* reward = NULL;
    srand((unsigned int)time(0));
    int score = 0;
    int frameCount = 0;
    int spawnInterval = 80;
    const int MAX_OBSTACLES = 5;

    Map map(renderer);
    Player player(renderer, selectedCar);

    std::vector<Obstacle*> obstacles;
    for (int i = 0; i < 3; i++) {
        obstacles.push_back(new Obstacle(renderer));
    }

    TTF_Font* font = TTF_OpenFont("img/fontchu.ttf", 24);
    if (!font) {
        SDL_Log("Can't open font': %s", TTF_GetError());
        return 1;
    }

    SDL_Color color = { 255, 255, 255 };
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
            player.handleEvent(e);
        }

        map.update();
        player.update();
        if (reward != NULL) {
    		reward->update();
		}

        frameCount++;
        if (frameCount >= spawnInterval && obstacles.size() < MAX_OBSTACLES) {
            int lanes[4] = {0, 1, 2, 3};
            shuffleLanes(lanes, 4);
            int numObstacles = rand() % 3 + 1;
            int spawned = 0;
            for (int i = 0; i < 4 && spawned < numObstacles; i++) {
                if (canSpawnInLane(lanes[i], obstacles)) {
                    Obstacle* obs = new Obstacle(renderer);
                    obs->resetAtLane(lanes[i]);
                    obstacles.push_back(obs);
                    spawned++;
                    
                    if (reward == NULL && !obstacles.empty()) {
    					int index = rand() % obstacles.size();
    					SDL_Rect obsRect = obstacles[index]->getRect();

    					reward = new Reward(renderer);
    					reward->setPosition(obsRect.x, obsRect.y + obsRect.h + 10);  // Ð?t reward ngay sau xe
					}
                }
            }
            frameCount = 0;
            if (spawnInterval > 30) spawnInterval--;
        }

        SDL_Rect playerRect = player.getRect();
        bool collided = false;

        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i]->update();

            SDL_Rect obstacleRect = obstacles[i]->getRect();
            int dx = abs(playerRect.x - obstacleRect.x);
            int dy = abs(playerRect.y - obstacleRect.y);

            if (dx < LANE_WIDTH - 20 && dy < CAR_HEIGHT - 30) {
                collided = true;
            }

            if (obstacles[i]->isPassed()) {
        		score++;
        		delete obstacles[i]; 
        		obstacles.erase(obstacles.begin() + i); 
        		i--; 
    }
        }

        if (collided) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Hit", "You crashed into another car", window);
            running = false;
        }

        SDL_RenderClear(renderer);
        map.render();
        player.render();

        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i]->render();
        }
        
        if (reward != NULL) {
    		reward->render();

    		SDL_Rect rewardRect = reward->getRect();
			if (SDL_HasIntersection(&playerRect, &rewardRect)) {
        		score += 5;
        		reward->collect();
    		}

    		if (reward->isCollected() || reward->isOutOfScreen()) {
     		   delete reward;
        		reward = NULL;
    		}
		}

        SDL_Rect scoreBox = { 10, 100, 120, 60 };
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 128);
        SDL_RenderFillRect(renderer, &scoreBox);

        char scoreText[30];
        sprintf(scoreText, "Score: %d", score);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, scoreText, color);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_RenderCopy(renderer, textTexture, NULL, &scoreBox);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for (size_t i = 0; i < obstacles.size(); i++) {
        delete obstacles[i];
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    if (reward != NULL) {
    	delete reward;
    	reward = NULL;
	}	
	Mix_FreeMusic(menuMusic);
	Mix_FreeMusic(gameMusic);
	Mix_CloseAudio();

    return 0;
}

