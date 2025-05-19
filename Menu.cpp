#include "Menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) {
    this->renderer = renderer;
    selectedCar = 0;

    // V? trí nút menu chính
    rectStart = {180, 160, 240, 60};
    rectOptions = {180, 240, 240, 60};
    rectExit = {180, 320, 240, 60};

    // V? trí các xe (gi?a màn hình)
    for (int i = 0; i < 3; ++i) {
        carRects[i] = {120 + i * 120, 250, 80, 80};
    }

    loadMainMenuTextures();
    loadCarTextures();
}

Menu::~Menu() {
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(btnStart);
    SDL_DestroyTexture(btnOptions);
    SDL_DestroyTexture(btnExit);

    for (int i = 0; i < 3; ++i) {
        SDL_DestroyTexture(carTextures[i]);
    }
}

bool Menu::loadMainMenuTextures() {
    background = IMG_LoadTexture(renderer, "img/menu_bg.png");
    btnStart = IMG_LoadTexture(renderer, "img/btn_start.png");
    btnOptions = IMG_LoadTexture(renderer, "img/btn_options.png");
    btnExit = IMG_LoadTexture(renderer, "img/btn_exit.png");

    return background && btnStart && btnOptions && btnExit;
}

bool Menu::loadCarTextures() {
    carTextures[0] = IMG_LoadTexture(renderer, "img/car1.png");
    carTextures[1] = IMG_LoadTexture(renderer, "img/car2.png");
    carTextures[2] = IMG_LoadTexture(renderer, "img/car3.png");

    return carTextures[0] && carTextures[1] && carTextures[2];
}

void Menu::renderMainMenu() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_RenderCopy(renderer, btnStart, NULL, &rectStart);
    SDL_RenderCopy(renderer, btnOptions, NULL, &rectOptions);
    SDL_RenderCopy(renderer, btnExit, NULL, &rectExit);
    SDL_RenderPresent(renderer);
}

int Menu::showMainMenu() {
    SDL_Event e;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return MENU_EXIT;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (isInside(x, y, rectStart)) return MENU_START;
                if (isInside(x, y, rectOptions)) return MENU_OPTIONS;
                if (isInside(x, y, rectExit)) return MENU_EXIT;
            }
        }
        renderMainMenu();
        SDL_Delay(16);
    }
    return MENU_NONE;
}

void Menu::renderCarMenu() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    for (int i = 0; i < 3; ++i) {
        SDL_SetTextureColorMod(carTextures[i], 255, 255, 255);
        if (i == selectedCar)
            SDL_SetTextureColorMod(carTextures[i], 255, 255, 100); // highlight
        SDL_RenderCopy(renderer, carTextures[i], NULL, &carRects[i]);
    }
    SDL_RenderPresent(renderer);
}

void Menu::showCarSelectMenu() {
    SDL_Event e;
    bool selecting = true;

    while (selecting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) selecting = false;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_LEFT)
                    selectedCar = (selectedCar + 2) % 3;
                if (e.key.keysym.sym == SDLK_RIGHT)
                    selectedCar = (selectedCar + 1) % 3;
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE)
                    selecting = false;
            }
        }
        renderCarMenu();
        SDL_Delay(16);
    }
}

int Menu::getSelectedCar() {
    return selectedCar;
}

bool Menu::isInside(int x, int y, SDL_Rect rect) {
    return (x > rect.x && x < rect.x + rect.w &&
            y > rect.y && y < rect.y + rect.h);
}

