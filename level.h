#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include "enemy.h"
#include "boss.h"
#include "factory.h"

// Класс уровня
class Level {
public:
    SDL_Color backgroundColor;
    std::vector<enemy*> enemies; // Список врагов
    boss* levelBoss;             // Босс (может быть nullptr)

    // Конструктор уровня
    Level(SDL_Color color) : backgroundColor(color), levelBoss(nullptr) {}

    // Очистка памяти при уничтожении уровня
    ~Level() {
        for (auto enemy : enemies) {
            delete enemy;
        }
        if (levelBoss) {
            delete levelBoss;
        }
    }

    // Метод отрисовки уровня
    void draw(SDL_Renderer* renderer) {
        // Устанавливаем цвет фона уровня
        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_RenderClear(renderer);

        // Отрисовка всех врагов
        for (auto enemy : enemies) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный для врагов
            SDL_RenderFillRect(renderer, &enemy->rect);
        }

        // Отрисовка босса (если есть)
        if (levelBoss) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Желтый для босса
            SDL_RenderFillRect(renderer, &levelBoss->rect);
        }
    }

    // Статический метод генерации случайного уровня
    static Level* generateRandomLevel(int windowWidth, int windowHeight) {
        // Случайный цвет фона
        SDL_Color color = {static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), static_cast<Uint8>(rand() % 256), 255};
        Level* level = new Level(color);

        // Генерация случайного количества врагов
        int numEnemies = 3 + rand() % 8; // От 3 до 10 врагов
        for (int i = 0; i < numEnemies; i++) {
            int x = rand() % (windowWidth - 50); // Случайная позиция по X
            int y = rand() % (windowHeight / 2); // Случайная позиция по Y
            level->enemies.push_back((enemy*)GameObjectFactory::createObject(ENEMY, x, y, 50, 50));
        }

        // Случайный босс (50% шанс появления)
        if (rand() % 2 == 0) {
            level->levelBoss = (boss*)GameObjectFactory::createObject(BOSS, windowWidth / 2, 50, 100, 100);
        }

        return level;
    }
};

#endif
