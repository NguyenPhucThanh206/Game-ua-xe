#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>


enum MenuChoice {
    MENU_NONE = 0,
    MENU_START,
    MENU_OPTIONS,
    MENU_EXIT
};

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    int showMainMenu();

    void showCarSelectMenu();

    int getSelectedCar();

private:
    SDL_Renderer* renderer;


    SDL_Texture* background;
    SDL_Texture* btnStart;
    SDL_Texture* btnOptions;
    SDL_Texture* btnExit;


    SDL_Texture* carTextures[3];


    SDL_Rect rectStart;
    SDL_Rect rectOptions;
    SDL_Rect rectExit;

    SDL_Rect carRects[3];
    int selectedCar;
	
	bool isInside(int x, int y, SDL_Rect rect);
    bool loadMainMenuTextures();
    bool loadCarTextures();
    void renderMainMenu();
    void renderCarMenu();
};
#endif
