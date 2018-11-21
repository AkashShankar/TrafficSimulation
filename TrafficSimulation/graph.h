#ifndef GRAPH_H
#define GRAPH_H

#include <SDL.h>
#include <vector>
#include <queue>

#include "utilities.h"

struct Entity;
struct EntitySystem;
struct VirtualGrid;
struct Display;
struct Text;
struct Editor;
struct Camera;

struct Node
{
    Entity *en = nullptr;
    std::vector<Entity*> adjacent_nodes;

    // also have the vector of distance to other nodes.
    // std::vector<int> distances; in the same order as adjacent_nodes
};

struct Graph
{
    EntitySystem *e_sys = nullptr; // set manually
    std::vector<Node*> nodes;

	 std::map<std::map<int, int>, std::vector<int>> final_occ_conn;
	 std::vector<std::vector<int>> adj_matrix;
	 std::map<int, int> occ_index; // maps index to occ
	 std::map<int, int> index_occ; // maps occ to index

	 /* for drawing edges */
	 Node *current_node = nullptr;
	 int current_node_index = -1;
	 std::vector<int> adjacent_d_node_indices;
	 SDL_Rect highlight_rect_pos { 0, 0, 0, 0 };
	 /* for drawing edges */

	 /* for highlighting path */
	 std::vector<int> grid_path_indices;
	 /* for highlighting path */

	 /* for highlighting car ( tmp ) path */
	 std::vector<int> vehicle_grid_indices;
	 bool src_clicked = false;
	 bool des_clicked = false;
	 int car_src_index = -1;
	 int car_des_index = -1;
	 SDL_Rect car_src_rect, car_des_rect;
	 Direction path_direction = Direction::NONE;
	 /* for highlighting car ( tmp ) path */

	 void init();
	 void destroy();
	 void add_all_nodes(EntitySystem *e_sys, VirtualGrid *v_grid);
	 std::vector<SDL_Point> compute_shortest_path_bw(VirtualGrid *v_grid, Camera *cam);
	 void set_car_src_des_indices(Editor *ed, VirtualGrid *grid, Camera *cam, int x, int y);
	 void draw_the_path(SDL_Renderer *ren, VirtualGrid *v_grid);
	 int get_min_key(std::vector<bool> &vtd, std::vector<int> &dist);
	 Node* get_node_with_id(int id);
	 void show_all_node_details();
	 void check_and_set_current_node(int x, int y);
	 void move_car_src_des_index_with_cam(Camera *cam, Grid *grid, VirtualGrid *v_grid);

	 void compute_adjacency_matrix();

	private:
	 void add_node(Entity *en, VirtualGrid *v_grid);
};

#endif
