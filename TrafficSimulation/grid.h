#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SDL.h>

#include "utilities.h"
#include "text.h"

struct Grid
{
	int x_pos = 0, y_pos = 0;
	int per_width = 50;
	int per_height = 50;
	int num_rows = 0;
	int num_cols = 0;
	int grid_width = 0;
	int grid_height = 0;
	std::vector<SDL_Point> points;

	void init(int num_rows, int num_cols);
	void draw(SDL_Renderer *ren);
	SDL_Point get_grid_pos(int x, int y);
	int get_index(int x, int y);
};

struct VirtualGrid
{
	int per_width = 50;
	int per_height = 50;
	int virtual_grid_width = 0;
	int virtual_grid_height = 0;

	int virtual_num_rows = 0;
	int virtual_num_cols = 0;

	std::vector<int> indices;
	std::vector<bool> filled_indices;
	std::vector<Text*> texts;
	std::vector<SDL_Rect> text_pos;

	void init(SDL_Renderer *ren, int grid_width, int grid_height);
	void show_index(int x, int y);
	void set_filled(int x, int y, int num_rows, int num_cols);
	void draw(SDL_Renderer *ren);
	void move_text_pos(Direction dir);
	void set_to_empty(int x, int y, int num_rows, int num_cols);
	std::vector<int> get_indices(int x, int y, int num_rows, int num_cols);
	int get_index(int x, int y);
	bool is_filled(int x, int y, int num_rows, int num_cols);
	std::vector<int> get_adjacent_indices(int index, int x_index_len, int y_index_len);
	Direction get_index_direction(int from_index, int to_index);
	SDL_Point get_pos_from_index(int v_grid_index);
	SDL_Point bound_pos_to_grid_dims(Grid *grid, int x, int y);

	void destroy();

	// tmp
	void show_filled_indices();
};

#endif
