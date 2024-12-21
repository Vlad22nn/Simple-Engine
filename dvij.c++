#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include "game.h"
#include "enemy_strategy.h"

using namespace std;

// Константы окна
const int windowWidth = 1600;
const int windowHeight = 1200;

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Game with Bonuses",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(time(0)); // Генерация случайных чисел

    // Создаём уровни
    vector<Level*> levels;
    for (int i = 0; i < 3; i++) {
        levels.push_back(Level::generateRandomLevel(windowWidth, windowHeight));
    }

    player* hero = (player*)GameObjectFactory::createObject(PLAYER, windowWidth / 2, windowHeight / 2, 50, 50);

    // Список бонусов
    vector<Bonus*> bonuses;

    // Создаём врагов с новыми стратегиями
    vector<enemy*> enemies;
    enemies.push_back(new enemy(100, 100, 50, 50, new AggressiveStrategy()));
    enemies.push_back(new enemy(200, 200, 50, 50, new DefensiveStrategy()));
    enemies.push_back(new enemy(300, 300, 50, 50, new ZigZagStrategy()));
    enemies.push_back(new enemy(400, 400, 50, 50, new RandomStrategy()));

    int currentLevel = 0;
    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        hero->move(keys);

        // Проверка столкновений с врагами
        for (auto enemy : levels[currentLevel]->enemies) {
            if (checkCollision(hero->rect, enemy->rect)) {
                hero->takeDamage(10);
                enemy->takeDamage(20);
            }
        }

        // Удаление врагов с 0 здоровья
        levels[currentLevel]->enemies.erase(
           remove_if(levels[currentLevel]->enemies.begin(),
                           levels[currentLevel]->enemies.end(),
                           [](enemy* e) {
                               if (e->health <= 0) {
                                   delete e;
                                   return true;
                               }
                               return false;
                           }),
            levels[currentLevel]->enemies.end());

        // Проверка здоровья босса
        if (levels[currentLevel]->levelBoss) {
            if (levels[currentLevel]->levelBoss->health <= 0) {
                // Порождаем бонус на месте босса
                bonuses.push_back(new Bonus(levels[currentLevel]->levelBoss->rect.x,
                                            levels[currentLevel]->levelBoss->rect.y,
                                            30, 30, (rand() % 2 == 0) ? HEALTH : SPEED));
                delete levels[currentLevel]->levelBoss;
                levels[currentLevel]->levelBoss = nullptr;
            } else if (checkCollision(hero->rect, levels[currentLevel]->levelBoss->rect)) {
                hero->takeDamage(20);
                levels[currentLevel]->levelBoss->takeDamage(10);
            }
        }

        // Проверка сбора бонусов
        for (auto bonus : bonuses) {
            if (!bonus->collected && checkCollision(hero->rect, bonus->rect)) {
                bonus->collected = true;
                switch (bonus->type) {
                    case HEALTH:
                        hero->health += 50;
                        if (hero->health > hero->maxHealth) hero->health = hero->maxHealth;
                        break;
                    case SPEED:
                        hero->speed += 2;
                        break;
                }
            }
        }

        // Удаление собранных бонусов
        bonuses.erase(
           remove_if(bonuses.begin(), bonuses.end(),
                           [](Bonus* b) {
                               if (b->collected) {
                                   delete b;
                                   return true;
                               }
                               return false;
                           }),
            bonuses.end());

        // Переход между уровнями
        if (hero->rect.x + hero->rect.w >= windowWidth) {
            currentLevel = (currentLevel + 1) % levels.size();
            hero->rect.x = 0;
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer,
                               levels[currentLevel]->backgroundColor.r,
                               levels[currentLevel]->backgroundColor.g,
                               levels[currentLevel]->backgroundColor.b,
                               levels[currentLevel]->backgroundColor.a);
        SDL_RenderClear(renderer);

        // Отрисовка уровня
        levels[currentLevel]->draw(renderer);

        // Отрисовка игрока и полоски здоровья
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &hero->rect);
        hero->drawHealthBar(renderer);

        // Отрисовка врагов
        for (auto enemy : levels[currentLevel]->enemies) {
            enemy->move(windowWidth, windowHeight); // Передача размеров окна
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &enemy->rect);
            enemy->drawHealthBar(renderer);
        }

        // Отрисовка босса
        if (levels[currentLevel]->levelBoss) {
            levels[currentLevel]->levelBoss->move(windowWidth, windowHeight);
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &levels[currentLevel]->levelBoss->rect);
            levels[currentLevel]->levelBoss->drawHealthBar(renderer);
        }

        // Отрисовка бонусов
        for (auto bonus : bonuses) {
            if (!bonus->collected) {
                bonus->draw(renderer);
            }
        }

        SDL_RenderPresent(renderer);

        // Ограничение FPS
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Очистка ресурсов
    for (auto level : levels) delete level;
    for (auto bonus : bonuses) delete bonus;
    for (auto enemy : enemies) delete enemy;
    delete hero;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
