#ifndef LEVEL_CREATOR_H
#define LEVEL_CREATOR_H

#include <SDL2/SDL.h>
#include <vector>
#include "level.h"
#include "factory.h"

class LevelCreator {
public:
    virtual ~LevelCreator() = default;

    // Шаблонный метод
    Level* createLevel() {
        Level* level = new Level(getBackgroundColor());
        populateEnemies(level); // Добавляем врагов
        addBoss(level);         // Добавляем босса, если есть
        return level;
    }

protected:
    virtual SDL_Color getBackgroundColor() = 0; // Цвет фона
    virtual void populateEnemies(Level* level) = 0; // Враги
    virtual void addBoss(Level* level) {}          // Босс (необязательно)
};

#endif
