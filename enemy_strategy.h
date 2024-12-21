
#ifndef ENEMY_STRATEGY_H
#define ENEMY_STRATEGY_H

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Базовый класс стратегии
class EnemyStrategy {
public:
    virtual void move(SDL_Rect& rect, int windowWidth, int windowHeight) = 0; // Добавлены размеры окна
};

// Агрессивная стратегия (движение вправо с фиксированной скоростью)
class AggressiveStrategy : public EnemyStrategy {
public:
    void move(SDL_Rect& rect, int windowWidth, int windowHeight) override {
        rect.x += 2;
        rect.y += 1;

        // Проверка на выход за границы
        if (rect.x < 0 || rect.x + rect.w > windowWidth || rect.y < 0 || rect.y + rect.h > windowHeight) {
            rect.x = std::max(0, std::min(rect.x, windowWidth - rect.w));
            rect.y = std::max(0, std::min(rect.y, windowHeight - rect.h));
        }
    }
};

// Защитная стратегия (движение влево с фиксированной скоростью)
class DefensiveStrategy : public EnemyStrategy {
public:
    void move(SDL_Rect& rect, int windowWidth, int windowHeight) override {
        rect.x -= 1;
        rect.y += 1;

        // Проверка на выход за границы
        if (rect.x < 0 || rect.x + rect.w > windowWidth || rect.y < 0 || rect.y + rect.h > windowHeight) {
            rect.x = std::max(0, std::min(rect.x, windowWidth - rect.w));
            rect.y = std::max(0, std::min(rect.y, windowHeight - rect.h));
        }
    }
};

// Зигзагообразное движение
class ZigZagStrategy : public EnemyStrategy {
private:
    int direction = 1; // Направление: 1 - вправо, -1 - влево
public:
    void move(SDL_Rect& rect, int windowWidth, int windowHeight) override {
        rect.x += direction * 2;
        rect.y += 1;

        // Меняем направление, если враг достигает границ экрана
        if (rect.x <= 0 || rect.x + rect.w >= windowWidth) {
            direction *= -1;
        }

        if (rect.y + rect.h > windowHeight) {
            rect.y = windowHeight - rect.h;
        }
    }
};

// Случайное движение
class RandomStrategy : public EnemyStrategy {
public:
    void move(SDL_Rect& rect, int windowWidth, int windowHeight) override {
        rect.x += (rand() % 3 - 1) * 2; // Случайное изменение по x (-2, 0, 2)
        rect.y += (rand() % 2 + 1);     // Медленное движение вниз

        // Проверка на выход за границы
        rect.x = std::max(0, std::min(rect.x, windowWidth - rect.w));
        rect.y = std::max(0, std::min(rect.y, windowHeight - rect.h));
    }
};

#endif
