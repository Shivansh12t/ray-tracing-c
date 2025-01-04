#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define W_WIDTH 1080
#define W_HEIGHT 600

#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_RAY 0xffd43b

#define RAYS_NUMBER 1000
#define RAY_THICKNESS 5

struct Circle{
    double x;
    double y;
    double radius;
};

struct Ray{
    double x_start;
    double y_start;
    double angle;
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

void generateRays(struct Circle circle, struct Ray rays[RAYS_NUMBER]){
    for (int i = 0; i < RAYS_NUMBER; i++){
        double angle = ( (double) i / RAYS_NUMBER ) * M_PI * 2;
        struct Ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;
        // printf("angle : %f\n", angle);
    }
}

// y = mx + c
// y = y_start - m * x_start + mx

void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color){
    int object_hit = 0;
    double step = 0.5;
    for (int i = 0; i < RAYS_NUMBER; i++){
        double tan_angle = tan(rays[i].angle);
        for (double x = 0; x < W_WIDTH; x += step){
            double y = rays[i].y_start - tan_angle * (x - rays[i].x_start);
            if (!object_hit && y < W_HEIGHT){
                SDL_Rect pixel = (SDL_Rect) {x,y,RAY_THICKNESS,RAY_THICKNESS};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

void FillRays_HD(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color, struct Circle object){
    double radius_squared = pow(object.radius, 2);
    
    for (int i = 0; i < RAYS_NUMBER; i++){
        struct Ray ray = rays[i];

        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;

        double cos_angle = cos(ray.angle);
        double sin_angle = sin(ray.angle);

        while ( !end_of_screen && !object_hit){
            x_draw += step*cos_angle;
            y_draw += step*sin_angle;

            SDL_Rect pixel = (SDL_Rect) {x_draw,y_draw,1,1};
            SDL_FillRect(surface, &pixel, color);

            if (x_draw < 0 || x_draw > W_WIDTH) end_of_screen = 1;
            if (y_draw < 0 || y_draw > W_HEIGHT) end_of_screen = 1;

            // Does ray hit an object
            double distance_squared = pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);
            if (distance_squared < radius_squared) object_hit = 1;
        }
    }
}

void move_shadow_circle(struct Circle* shadow_circle, int* obj_speed){
    shadow_circle->y += (double) *obj_speed;
    if (shadow_circle->y < 0.0 || shadow_circle->y >= W_HEIGHT) *obj_speed = -*obj_speed;
}

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Raytracing in C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = (struct Circle){200, 200, 40};
    struct Circle shadow_circle = (struct Circle){700, 300, 120};
    SDL_Rect erase_rect = (SDL_Rect){0,0,W_WIDTH, W_HEIGHT};

    struct Ray rays[RAYS_NUMBER];
    generateRays(circle, rays);

    int simulation_running = 1;
    int object_speed = 1;
    SDL_Event event;

    while ( simulation_running ){

        // if you dont do this, you wont be able to quit the application !
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                simulation_running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0){
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generateRays(circle, rays);
            }
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

        FillCircle(surface, shadow_circle, COLOR_WHITE);
        FillRays_HD(surface, rays, COLOR_RAY, shadow_circle);
        FillCircle(surface, circle, COLOR_WHITE);

        move_shadow_circle(&shadow_circle, &object_speed);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_Delay(5000);
    return 0;
}