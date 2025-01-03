#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define W_WIDTH 900
#define W_HEIGHT 600

#define COLOR_WHITE 0xffffffff

struct Circle{
    double x;
    double y;
    double radius;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color){
    double radius_squared = pow(circle.radius,2);
    for (double x = circle.x - circle.radius; x <= circle.x + circle.radius; x++){
        for (double y = circle.y - circle.radius; y <= circle.y + circle.radius; y++){
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared){
                SDL_Rect pixel = (SDL_Rect) {x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Raytracing in C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // SDL_Rect rect = (SDL_Rect) {200,200,200,200};
    // SDL_FillRect(surface, &rect, COLOR_WHITE);

    struct Circle circle = (struct Circle){200, 200, 80};
    FillCircle(surface, circle, COLOR_WHITE);

    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000);
    return 0;
}