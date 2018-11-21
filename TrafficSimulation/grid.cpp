#include "pch.h"
#include "Grid.h"
#include "camera.h"

#include <iostream>

void Grid::init(int num_rows, int num_cols)
{
    this->num_rows = num_rows;
    this->num_cols = num_cols;

    grid_width = per_width * num_cols;
    grid_height = per_height * num_rows;

    for (int i = 0; i < this->num_rows; i++)
    {
        for (int j = 0; j < this->num_cols; j++)
        {
            SDL_Point tmp{j * per_width, i * per_height};
            points.push_back(tmp);
        }
    }
}

void Grid::draw(SDL_Renderer *ren)
{
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 150, 60);

    for (unsigned int i = 0; i < points.size(); i += num_cols)
        SDL_RenderDrawLine(ren, points[i].x, points[i].y, points[i + num_cols - 1].x, points[i + num_cols - 1].y);

    for (int i = 0; i < num_cols; i++)
        SDL_RenderDrawLine(ren, points[i].x, points[i].y, points[i].x, points[0].y + grid_height - per_height);
}

SDL_Point Grid::get_grid_pos(int x, int y)
{
    int in = get_index(x, y);

    return points[in];
}

int Grid::get_index(int x, int y)
{
    double w_percent = (float)x / grid_width;
    double h_percent = (float)y / grid_height;

    int current_col = (int)(w_percent * num_cols);
	int current_row = (int)(h_percent * num_rows);

    int in = current_row * num_cols + current_col;

    return in;
}

void VirtualGrid::init(SDL_Renderer *ren, int grid_width, int grid_height)
{
    virtual_grid_width = grid_width;
    virtual_grid_height = grid_height;

    virtual_num_cols = grid_width / per_width;
    virtual_num_rows = grid_height / per_height;

    int total = virtual_num_cols * virtual_num_rows;
    for (int i = 0; i < total; i++)
    {
        indices.push_back(i);
        filled_indices.push_back(false);

        Text *tmp_text = new Text;
        tmp_text->col = {0, 0, 0, 150};
        std::string tmp_str_text = std::to_string(i);
        tmp_text->init(ren, "Assets\\times.ttf", tmp_str_text, 15);
        texts.push_back(tmp_text);
    }

    for (int i = 0; i < virtual_num_rows; i++)
    {
        for (int j = 0; j < virtual_num_cols; j++)
        {
            int tmp_in = i * virtual_num_cols + j;
            int text_width = 0, text_height = 0;
            SDL_QueryTexture(texts[tmp_in]->tex, NULL, NULL, &text_width, &text_height);
            SDL_Rect tmp_rect{j * per_width + 5, i * per_height + 5, text_width, text_height};
            text_pos.push_back(tmp_rect);
        }
    }
}

SDL_Point VirtualGrid::bound_pos_to_grid_dims(Grid *grid, int x, int y)
{
    SDL_Point conv_pos;

    int x_times = x / grid->grid_width;
    conv_pos.x = x - x_times * grid->grid_width;

    int y_times = y / grid->grid_height;
    conv_pos.y = y - y_times * grid->grid_height;

    return conv_pos;
}

SDL_Point VirtualGrid::get_pos_from_index(int v_grid_index)
{
    int x_index = v_grid_index % virtual_num_cols;

    if (x_index == 0)
        x_index++;

    int y_index = (v_grid_index - x_index) / virtual_num_rows;

    float x_percent = (x_index) / (float)virtual_num_cols;
    float y_percent = (y_index) / (float)virtual_num_rows;
    SDL_Point tmp_pos{(int)(x_percent * virtual_grid_width), (int)(y_percent * virtual_grid_height)};

    return tmp_pos;
}

void VirtualGrid::move_text_pos(Direction dir)
{
    int quantity_x = 0;
    int quantity_y = 0;
    if (dir == Direction::LEFT)
        quantity_x = +per_width;
    else if (dir == Direction::RIGHT)
        quantity_x = -per_width;
    else if (dir == Direction::UP)
        quantity_y = +per_height;
    else if (dir == Direction::DOWN)
        quantity_y = -per_height;

    for (unsigned int i = 0; i < text_pos.size(); i++)
    {
        text_pos[i].x += quantity_x;
        text_pos[i].y += quantity_y;
    }
}

void VirtualGrid::draw(SDL_Renderer *ren)
{
    for (unsigned int i = 0; i < texts.size(); i++)
    {
        SDL_SetTextureAlphaMod(texts[i]->tex, 250);
        SDL_RenderCopy(ren, texts[i]->tex, NULL, &text_pos[i]);
    }
}

int VirtualGrid::get_index(int x, int y)
{
    double w_percent = (float)x / virtual_grid_width;
    double h_percent = (float)y / virtual_grid_height;

	int current_col = (int)(w_percent * virtual_num_cols);
	int current_row = (int)(h_percent * virtual_num_rows);

    int in = current_row * virtual_num_cols + current_col;

    return in;
}

void VirtualGrid::show_index(int x, int y)
{
    int in = get_index(x, y);
    std::cout << "in: " << in << std::endl;
}

void VirtualGrid::set_filled(int x, int y, int num_rows, int num_cols)
{
	 // x and y needs to be within (window_width and window_height)
	if(x < 0 || y < 0)
	{
		std::cout << "Error VirtualGrid::set_filled() < 0: " << x << ", " << y;
		std::cout << std::endl;
		return;
	}

    int in = get_index(x, y); 
    for (int i = 0; i < num_rows; i++)
        for (int j = 0; j < num_cols; j++)
            filled_indices[j + in + i * virtual_num_cols] = true;
}

void VirtualGrid::set_to_empty(int x, int y, int num_rows, int num_cols)
{
    int in = get_index(x, y);

    for (int i = 0; i < num_rows; i++)
        for (int j = 0; j < num_cols; j++)
            filled_indices[j + in + i * virtual_num_cols] = false;
}

std::vector<int> VirtualGrid::get_indices(int x, int y, int num_rows, int num_cols)
{
    std::vector<int> indices;
    int in = get_index(x, y);
    for (int i = 0; i < num_rows; i++)
        for (int j = 0; j < num_cols; j++)
            indices.push_back(j + in + i * virtual_num_cols);

    return indices;
}

bool VirtualGrid::is_filled(int x, int y, int num_rows, int num_cols)
{
    int in = get_index(x, y);
    for (int i = 0; i < num_rows; i++)
        for (int j = 0; j < num_cols; j++)
            if (filled_indices[j + in + i * virtual_num_cols] == true)
                return true;

    return false;
}

std::vector<int> VirtualGrid::get_adjacent_indices(int index, int x_index_len, int y_index_len)
{
    // Note: 0:left, 1:right, 2:down, 3:up
    std::vector<int> ins;
    if (index - 1 >= 0)
        ins.push_back(index - 1); // left
    else
        ins.push_back(-1);

    if (index + x_index_len <= virtual_num_cols * virtual_num_rows)
        ins.push_back(index + x_index_len); // right
    else
        ins.push_back(-1);

    if (index + y_index_len + virtual_num_cols <= virtual_num_cols * virtual_num_rows)
        ins.push_back(index + y_index_len * virtual_num_cols); // down
    else
        ins.push_back(-1);

    if (index - virtual_num_cols >= 0)
        ins.push_back(index - virtual_num_cols); // up
    else
        ins.push_back(-1);

    return ins;
}

Direction VirtualGrid::get_index_direction(int from_index, int to_index)
{
    Direction dir = Direction::NONE;

    if (to_index > from_index && abs(to_index - from_index) <= virtual_num_cols)
        dir = Direction::RIGHT;
    else if (to_index > from_index && abs(to_index - from_index) >= virtual_num_cols)
        dir = Direction::DOWN;
    else if (to_index < from_index && abs(to_index - from_index) <= virtual_num_cols)
        dir = Direction::LEFT;
    else if (to_index < from_index && abs(to_index - from_index) >= virtual_num_cols)
        dir = Direction::UP;

    return dir;
}

void VirtualGrid::destroy()
{
    for (unsigned int i = 0; i < texts.size(); i++)
        texts[i]->destroy();
}

void VirtualGrid::show_filled_indices()
{
    std::cout << "filled_indices are: " << std::endl;
    for (unsigned int i = 0; i < filled_indices.size(); i++)
    {
        if (filled_indices[i])
            std::cout << indices[i] << " ";
    }
    std::cout << std::endl
              << "----------------" << std::endl;
    std::cout << std::endl;
}
