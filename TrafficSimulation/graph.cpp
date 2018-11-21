#include "pch.h"
#include "graph.h"
#include "entity.h"
#include "utilities.h"
#include "grid.h"
#include "display.h"
#include "text.h"
#include "editor.h"
#include "camera.h"

#include <assert.h>
#include <algorithm>
#include <climits>

void Graph::init()
{
	assert(e_sys != nullptr);

	highlight_rect_pos.w = 40;
	highlight_rect_pos.h = 40;
}

void Graph::add_node(Entity *en, VirtualGrid *v_grid)
{
	if (!en)
		return;

	Node *tmp_node = nullptr;
	tmp_node = new Node;
	tmp_node->en = en;

	if(is_en_type_a_vehicle(tmp_node->en->type) || tmp_node->en->type == EntityType::PERSON)
		return;

	SDL_Point num_rc = en->num_rows_cols;
	std::vector<Entity*> tmp_ens = e_sys->get_entities_surrounding_entity(en, v_grid);

	/*
	std::cout << "add_node: ";
	print_entity_type(en->type);
	std::cout << std::endl;
	*/

	/* Note: Here we need all non-movable entities that are surrounding en */

	for(unsigned int i = 0; i < tmp_ens.size(); i++)
	{
		if(tmp_ens[i] && !is_en_type_a_vehicle(tmp_ens[i]->type))
		{
			if (if_connected(e_sys, v_grid, en, tmp_ens[i]))
			{
				tmp_node->adjacent_nodes.push_back(tmp_ens[i]);
			}
		}
	}

	nodes.push_back(tmp_node);
}

void Graph::compute_adjacency_matrix()
{
	// connecting the individual en->occ_conn
	/* Note: Do not connect two entities twice, because one connect_en()
	 * ensures both get connected */

	final_occ_conn.clear();
	adj_matrix.clear();
	occ_index.clear();
	index_occ.clear();

	std::vector<int> done_ids;
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		Entity *src_en = nodes[i]->en;

		for(unsigned int j = 0; j < nodes[i]->adjacent_nodes.size(); j++)
		{
			Entity *to_en = nodes[i]->adjacent_nodes[j];
			src_en->connect_en(to_en);
		}
	}

	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		std::map<std::map<int, int>, std::vector<int>>::iterator tmp_it = nodes[i]->en->occ_conn.begin();

		for(; tmp_it != nodes[i]->en->occ_conn.end(); ++tmp_it)
		{
			final_occ_conn[tmp_it->first] = tmp_it->second;
		}
	}

	std::cout << "total_nodes are: " << final_occ_conn.size() << std::endl;

	/*
	// tmp: displaying final_occ_conn
	std::cout << "final_occ_conn for src_index: " << car_src_index << std::endl;
	std::map<std::map<int, int>, std::vector<int>>::iterator it = final_occ_conn.begin();
	for(; it != final_occ_conn.end(); ++it)
	{
	std::map<int, int> tmp_m = it->first;
	std::map<int, int>::iterator in_it = tmp_m.begin();
	std::vector<int> tmp_vec = it->second;

	std::cout << in_it->second << " is: ";
	for(unsigned int i = 0; i < tmp_vec.size(); i++)
	{
	std::cout << tmp_vec[i] << " ";
	}
	std::cout << std::endl;
	}
	*/

	int count = 0;

	std::map<std::map<int, int>, std::vector<int>>::iterator tmp_it = final_occ_conn.begin();
	// Filling the occ_index
	for(; tmp_it != final_occ_conn.end(); ++tmp_it)
	{
		std::map<int, int> tmp_m = tmp_it->first;
		std::map<int, int>::iterator tmp_it = tmp_m.begin();
		occ_index[tmp_it->second] = count;
		index_occ[count] = tmp_it->second;
		count++;
	}

	/* Now form a new adj_matrix that connects the individual occupied_indices
	 * for all the entities */

	// filling all indices to 0
	for(unsigned int i = 0; i < final_occ_conn.size(); i++)
	{
		std::vector<int> tmp_vec;
		for(unsigned int j = 0; j < final_occ_conn.size(); j++)
			tmp_vec.push_back(0);

		adj_matrix.push_back(tmp_vec);
	}

	// Filling in the adj_matrix with edge weights
	tmp_it = final_occ_conn.begin();
	for(; tmp_it != final_occ_conn.end(); ++tmp_it)
	{
		// we need to figure out row and column index
		std::map<int, int> tmp_m = tmp_it->first;
		std::map<int, int>::iterator tmp_it_2 = tmp_m.begin();
		std::vector<int> in_vec = tmp_it->second;

		int row_index = occ_index[tmp_it_2->second];

		for(unsigned int i = 0; i < in_vec.size(); i++)
		{
			std::map<int, int>::iterator tmp_it_2 = tmp_m.begin();
			int col_index = occ_index[in_vec[i]];
			adj_matrix[row_index][col_index] = 1;
		}
	}

	/*
	// Displaying the adj_matrix
	for(unsigned int i = 0; i < final_occ_conn.size(); i++)
	{
	std::cout << i << ": ";
	for(unsigned int j = 0; j < final_occ_conn.size(); j++)
	{
	std::cout << adj_matrix[i][j] << "  ";
	}
	std::cout << std::endl;
	}
	*/
}

std::vector<SDL_Point> Graph::compute_shortest_path_bw(VirtualGrid *v_grid, Camera *cam)
{
	std::vector<SDL_Point> tmp_path_vec;
	if(!src_clicked || !des_clicked)
		return tmp_path_vec;

	/* Call compute_adj_matrix atleast once */
	/* Call compute_adj_matrix atleast once */

	// Dijkstra's alg.
	std::vector<bool> visited(final_occ_conn.size(), false);
	std::vector<int> distances(final_occ_conn.size(), INT_MAX);
	std::vector<int> parent(final_occ_conn.size(), -1);

	distances[occ_index[car_src_index]] = 0;
	parent[occ_index[car_src_index]] = -1;

	for (unsigned int i = 0; i < final_occ_conn.size() - 1; i++)
	{
		int u = get_min_key(visited, distances);
		visited[u] = true;

		for (unsigned int v = 0; v < final_occ_conn.size(); v++)
		{
			if (visited[v] == false && adj_matrix[u][v] != 0 &&
					distances[u] != INT_MAX && 
					(distances[u] + adj_matrix[u][v] < distances[v]))
			{
				distances[v] = adj_matrix[u][v] + distances[u];
				parent[v] = u;
			}
		}
	}

	grid_path_indices.erase(grid_path_indices.begin(), grid_path_indices.end());
	int tmp_parent = parent[occ_index[car_des_index]];

	while (tmp_parent != -1)
	{
		grid_path_indices.push_back(index_occ[tmp_parent]);
		tmp_parent = parent[tmp_parent];
	}

	// Note the path is reverse of grid_path_indices
	// Note the path is reverse of grid_path_indices

	/*
		std::cout << "all occ_en_connections" << std::endl;
		for(unsigned int i = 0; i < nodes.size(); i++)
		{
		show_all_occ_en(nodes[i]->en);
		}
		*/

	/*
	// std::cout << "total nodes are: " << final_occ_conn.size() << std::endl;
	std::cout << "The grid_path is for src_index: " << car_src_index << " is:";
	for(int i = grid_path_indices.size() - 1; i >= 0; i--)
	{
		std::cout << grid_path_indices[i] << " ";
	}
	std::cout << std::endl;
	*/

	/* Note: Here compute the mid_points of all the grid_path_indices from src to
	 * des and store then in a std::queue<SDL_Point> and return the std::queue.
	 */

	// std::cout << "grid_path_indices.size(): " << grid_path_indices.size() << std::endl;
	// std::cout << "mid_points are for src " << car_src_index << " are: ";
	for(int i = grid_path_indices.size() - 1; i >= 0; i--)
	{
		SDL_Point tmp_pos = v_grid->get_pos_from_index(grid_path_indices[i]);
		tmp_pos.x += v_grid->per_width / 2;
		tmp_pos.y += v_grid->per_height / 2;
		// std::cout << "(" << tmp_pos.x << "," << tmp_pos.y << "), ";
		tmp_path_vec.push_back(tmp_pos);
	}

	/*
	std::cout << std::endl;
	std::cout << "-----------------" << std::endl;
	*/

	return tmp_path_vec;
}

void Graph::set_car_src_des_indices(Editor *ed, VirtualGrid *grid, Camera *cam, int x, int y)
{
	// Note: First see if editor's en_nodes and highlighted and then go for car's src and des index.
	if(!ed->first_en_node || !ed->second_en_node)
		return;

	static int count = 0;
	count++;

	if(count > 2)
	{
		src_clicked = des_clicked = false;
		count = 0;
	}

	if(!src_clicked)
	{
		int tmp_grid_index = grid->get_index(x + cam->pos.x, y + cam->pos.y);
		car_src_index = tmp_grid_index;
		// Cheking if car_src_index belongs to ed->first_node->occupied_indices
		bool belongs = false;
		for(unsigned int i = 0; i < ed->first_en_node->occupied_indices.size(); i++) {
			if(car_src_index == ed->first_en_node->occupied_indices[i])
				belongs = true;
		}
		if(!belongs)
		{
			count--;
			return;
		}
		SDL_Point tmp_pos  = grid->get_pos_from_index(grid->get_index(x, y));
		car_src_rect = { tmp_pos.x, tmp_pos.y, grid->per_width, grid->per_height };
		src_clicked = true;
	}
	else if(!des_clicked)
	{
		int tmp_grid_index = grid->get_index(x + cam->pos.x, y + cam->pos.y);
		car_des_index = tmp_grid_index;
		// Cheking if car_des_index belongs to ed->second_node->occupied_indices
		bool belongs = false;
		for(unsigned int i = 0; i < ed->second_en_node->occupied_indices.size(); i++) {
			if(car_des_index == ed->second_en_node->occupied_indices[i])
				belongs = true;
		}
		if(!belongs)
		{
			count--;
			return;
		}
		SDL_Point tmp_pos  = grid->get_pos_from_index(grid->get_index(x, y));
		car_des_rect = { tmp_pos.x, tmp_pos.y, grid->per_width, grid->per_height };
		des_clicked = true;
	}
}

void Graph::move_car_src_des_index_with_cam(Camera *cam, Grid *grid, VirtualGrid *v_grid)
{
	int move_by_x = 0, move_by_y = 0;
	if(cam->current_dir_state == Direction::RIGHT)
		move_by_x = -grid->per_width;
	else if(cam->current_dir_state == Direction::LEFT)
		move_by_x = +grid->per_width;
	else if(cam->current_dir_state == Direction::DOWN)
		move_by_y = -grid->per_height;
	else if(cam->current_dir_state == Direction::UP)
		move_by_y = +grid->per_height;

	if(src_clicked)
	{
		car_src_rect.x += move_by_x;
		car_src_rect.y += move_by_y;
	}
	if(des_clicked)
	{
		car_des_rect.x += move_by_x;
		car_des_rect.y += move_by_y;
	}
}

void Graph::draw_the_path(SDL_Renderer *ren, VirtualGrid *v_grid)
{
	if(src_clicked)
	{
		SDL_SetRenderDrawColor(ren, 255, 255, 0, 200);
		SDL_RenderFillRect(ren, &car_src_rect); 
	}

	if(des_clicked)
	{
		SDL_SetRenderDrawColor(ren, 0, 255, 255, 200);
		SDL_RenderFillRect(ren, &car_des_rect); 
	}
}

int Graph::get_min_key(std::vector<bool> &vtd, std::vector<int> &dist)
{
	int min = INT_MAX;
	int min_index;
	for (unsigned int i = 0; i < vtd.size(); i++)
	{
		if (vtd[i] == false && dist[i] <= min)
		{
			min = dist[i];
			min_index = i;
		}
	}

	return min_index;
}

Node *Graph::get_node_with_id(int id)
{
	Node *tmp_node = nullptr;
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->en->id == id)
		{
			tmp_node = nodes[i];
			break;
		}
	}

	return tmp_node;
}

void Graph::add_all_nodes(EntitySystem *e_sys, VirtualGrid *v_grid)
{
	// Deletes all nodes and then adds them
	for (unsigned int i = 0; i < nodes.size(); i++)
		delete nodes[i];

	nodes.clear();

	for (unsigned int i = 0; i < e_sys->entities.size(); i++)
	{
		add_node(e_sys->entities[i], v_grid);
	}

	/* tmp */
	show_all_node_details();
	/* tmp */
}

void Graph::show_all_node_details()
{
	std::cout << std::endl
		<< " ----------------- " << std::endl;
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		std::cout << (i + 1) << ": ";
		std::cout << "Node with ";
		print_entity_type(nodes[i]->en->type);
		std::cout << " having id: "
			<< nodes[i]->en->id << " is connected to ";
		for (unsigned int j = 0; j < nodes[i]->adjacent_nodes.size(); j++)
			std::cout << nodes[i]->adjacent_nodes[j]->id << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl
		<< " ----------------- " << std::endl;
}

void Graph::destroy()
{
	for (unsigned int i = 0; i < nodes.size(); i++)
		delete nodes[i];
}
