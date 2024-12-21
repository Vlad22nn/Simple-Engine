#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class player {
public:
    SDL_Rect rect;
    SDL_Color color;
    int speed;
    int health;
    int maxHealth;

    player(int x, int y, int width, int height, SDL_Color color, int speed)
        : rect{x, y, width, height}, color(color), speed(speed), health(200), maxHealth(200) {}

    void move(const Uint8* keys) {
        if (keys[SDL_SCANCODE_DOWN] && (rect.y + rect.h + speed) <= 1200) {
            rect.y += speed;
        }
        if (keys[SDL_SCANCODE_UP] && (rect.y - speed) >= 0) {
            rect.y -= speed;
        }
        if (keys[SDL_SCANCODE_RIGHT] && (rect.x + rect.w + speed) <= 1600) {
            rect.x += speed;
        }
        if (keys[SDL_SCANCODE_LEFT] && (rect.x - speed) >= 0) {
            rect.x -= speed;
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void drawHealthBar(SDL_Renderer* renderer) {
        SDL_Rect backgroundBar = {rect.x, rect.y - 10, rect.w, 5};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный фон
        SDL_RenderFillRect(renderer, &backgroundBar);

        SDL_Rect healthBar = {rect.x, rect.y - 10, (rect.w * health) / maxHealth, 5};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Зелёный цвет
        SDL_RenderFillRect(renderer, &healthBar);
    }
};

#endif // PLAYER_H
