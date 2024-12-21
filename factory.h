#ifndef FACTORY_H
#define FACTORY_H

#include <SDL2/SDL.h>
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "enemy_strategy.h"

enum ObjectType { PLAYER, ENEMY, BOSS };

class GameObjectFactory {
public:
    static void* createObject(ObjectType type, int x, int y, int w, int h) {
        switch (type) {
            case PLAYER:
                return new player(x, y, w, h, {255, 255, 255, 255}, 5);
            case ENEMY:
                if (rand() % 2 == 0) {
                    return new enemy(x, y, w, h, new AggressiveStrategy());
                } else {
                    return new enemy(x, y, w, h, new DefensiveStrategy());
                }
            case BOSS:
                return new boss(x, y, w, h, new AggressiveStrategy());
            default:
                return nullptr;
        }
    }
};

#endif
