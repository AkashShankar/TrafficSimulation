#include "pch.h"
#include "camera.h"

void Camera::init(int x_limit, int y_limit)
{
    limit.x = x_limit;
    limit.y = y_limit;
}

void Camera::move_camera()
{
    if (current_dir_state == Direction::UP)
        pos.y -= cam_speed;
    else if (current_dir_state == Direction::DOWN)
        pos.y += cam_speed;
    else if (current_dir_state == Direction::LEFT)
        pos.x -= cam_speed;
    else if (current_dir_state == Direction::RIGHT)
        pos.x += cam_speed;
}

void Camera::move_point_with_camera(int &x, int &y)
{
    if (current_dir_state == Direction::UP)
        y += cam_speed;
    else if (current_dir_state == Direction::DOWN)
        y -= cam_speed;
    else if (current_dir_state == Direction::LEFT)
        x += cam_speed;
    else if (current_dir_state == Direction::RIGHT)
        x -= cam_speed;
}
