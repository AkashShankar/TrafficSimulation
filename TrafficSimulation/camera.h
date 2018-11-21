#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"
#include <SDL.h>

struct Camera
{
    SDL_Point pos{0, 0};
    SDL_Point limit { 0, 0, };
    Direction current_dir_state = Direction::NONE;
    int cam_speed = 1;

    void init(int x_limit, int y_limit);
    void move_camera();
    void move_point_with_camera(int &x, int &y);
};

#endif