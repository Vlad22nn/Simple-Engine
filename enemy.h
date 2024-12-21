
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "enemy_strategy.h"

class enemy {
public:
    SDL_Rect rect;
    EnemyStrategy* strategy;
    int health;
    int maxHealth;

    enemy(int x, int y, int w, int h, EnemyStrategy* strat)
        : rect{x, y, w, h}, strategy(strat), health(30), maxHealth(30) {}

    ~enemy() {
        delete strategy;
    }

    void move(int windowWidth, int windowHeight) {
        if (strategy) {
            strategy->move(rect, windowWidth, windowHeight);
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void drawHealthBar(SDL_Renderer* renderer) {
        SDL_Rect backgroundBar = {rect.x, rect.y - 10, rect.w, 5};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &backgroundBar);

        SDL_Rect healthBar = {rect.x, rect.y - 10, static_cast<int>((rect.w * health) / maxHealth), 5};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &healthBar);
    }
};

#endif
