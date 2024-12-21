#ifndef BONUS_H
#define BONUS_H

#include <SDL2/SDL.h>

enum BonusType { HEALTH, SPEED, SCORE };

class Bonus {
public:
    SDL_Rect rect;      
    BonusType type;      
    bool collected;      

    Bonus(int x, int y, int w, int h, BonusType type)
        : rect{x, y, w, h}, type(type), collected(false) {}


    void draw(SDL_Renderer* renderer) {
        switch (type) {
            case HEALTH: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break; 
            case SPEED: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
            case SCORE: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break; 
        }
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif
