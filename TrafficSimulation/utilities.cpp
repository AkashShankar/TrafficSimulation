#include "pch.h"
#include "utilities.h"
#include "camera.h"
#include "graph.h"
#include "grid.h"
#include "simulation.h"

#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>

Image* make_image(std::string str, SDL_Renderer *ren)
{
	Image *tmp_img = new Image;
	tmp_img->init(str, ren);

	return tmp_img;
}

void Utils::init_utils()
{
	en_ic_map[EntityType::ROAD] = IconType::ROAD;
	en_ic_map[EntityType::NONE] = IconType::NONE;
	en_ic_map[EntityType::ROAD_4_JUNC] = IconType::ROAD_4_JUNC;
	en_ic_map[EntityType::CAR] = IconType::ORANGE_CAR;
	en_ic_map[EntityType::JUNCTION] = IconType::JUNCTION;
	en_ic_map[EntityType::PAVEMENT] = IconType::PAVEMENT;
	en_ic_map[EntityType::PERSON] = IconType::PERSON;
	en_ic_map[EntityType::RECT_H] = IconType::RECT_H;
	en_ic_map[EntityType::RECT_V] = IconType::RECT_V;
	en_ic_map[EntityType::TRAFFIC_LIGHT] = IconType::TRAFFIC_LIGHT;
	en_ic_map[EntityType::BUS_STAND] = IconType::BUS_STAND;
	en_ic_map[EntityType::BUS] = IconType::BUS;

	ic_en_map[IconType::ROAD] = EntityType::ROAD;
	ic_en_map[IconType::NONE] = EntityType::NONE;
	ic_en_map[IconType::ROAD_4_JUNC] = EntityType::ROAD_4_JUNC;
	ic_en_map[IconType::ORANGE_CAR] = EntityType::CAR;
	ic_en_map[IconType::POLICE_CAR] = EntityType::CAR;
	ic_en_map[IconType::VIPER_CAR] = EntityType::CAR;
	ic_en_map[IconType::AUDI_CAR] = EntityType::CAR;
	ic_en_map[IconType::JUNCTION] = EntityType::JUNCTION;
	ic_en_map[IconType::PAVEMENT] = EntityType::PAVEMENT;
	ic_en_map[IconType::PERSON] = EntityType::PERSON;
	ic_en_map[IconType::RECT_H] = EntityType::RECT_H;
	ic_en_map[IconType::RECT_V] = EntityType::RECT_V;
	ic_en_map[IconType::TRAFFIC_LIGHT] = EntityType::TRAFFIC_LIGHT;
	ic_en_map[IconType::BUS_STAND] = EntityType::BUS_STAND;
	ic_en_map[IconType::BUS] = EntityType::BUS;

	type_rows_col_map[EntityType::ROAD] = SDL_Point { 2, 2 };
	type_rows_col_map[EntityType::JUNCTION] = SDL_Point { 2, 2 };
	type_rows_col_map[EntityType::ROAD_4_JUNC] = SDL_Point { 4, 4 };
	type_rows_col_map[EntityType::CAR] = SDL_Point { 1, 1 };
	type_rows_col_map[EntityType::BUS] = SDL_Point { 1, 1 };
	type_rows_col_map[EntityType::PAVEMENT] = SDL_Point { 1, 1 };
	type_rows_col_map[EntityType::PERSON] = SDL_Point { 1, 1 };
	type_rows_col_map[EntityType::RECT_H] = SDL_Point { 1, 2 };
	type_rows_col_map[EntityType::RECT_V] = SDL_Point { 2, 1 };
	type_rows_col_map[EntityType::TRAFFIC_LIGHT] = SDL_Point { 2, 2 };
	type_rows_col_map[EntityType::BUS_STAND] = SDL_Point { 2, 2 };
}

void RandomNumbers::init()
{
	gen = new std::mt19937(rd());
}

RandomNumbers::~RandomNumbers()
{
	if (gen)
		delete gen;
}

RandomNumbers::RandomNumbers()
{
	gen = new std::mt19937(rd());
}

std::vector<int> RandomNumbers::get_uniform_dis(int from, int to, int times)
{
	std::uniform_int_distribution<> dis(from, to);
	std::vector<int> tmp_ints;

	assert(gen != nullptr);

	for (int i = 0; i < times; i++)
		tmp_ints.push_back(dis(*gen));

	return tmp_ints;
}

int RandomNumbers::get_random_number(int from, int to)
{
	assert(gen != nullptr);
	std::uniform_int_distribution<> dis(from, to);

	return dis(*gen);
}

void RandomNumbers::print_uniform_dis(int from, int to, int times)
{
	std::uniform_int_distribution<> dis(from, to);

	std::cout << "--------------------" << std::endl;
	std::cout << "Random uniform dis values from " << from << " to " << to << " are: " << std::endl;
	for (int i = 0; i < times; i++)
		std::cout << (i + 1) << " : " << dis(*gen) << std::endl;
	std::cout << "--------------------" << std::endl;
}

IconType get_icon_type_from_entity_type(EntityType type, Utils *ut)
{
	return ut->en_ic_map[type];
}

EntityType get_entity_type_from_icon_type(IconType type, Utils *ut)
{
	return ut->ic_en_map[type];
}

EntityType get_type_from_str(std::string str)
{
	EntityType tmp_type = EntityType::NONE;
	if (str == "car")
		tmp_type = EntityType::CAR;
	else if (str == "road")
		tmp_type = EntityType::ROAD;
	else if (str == "junction")
		tmp_type = EntityType::JUNCTION;
	else if (str == "road_4_junc")
		tmp_type = EntityType::ROAD_4_JUNC;
	else if (str == "pavement")
		tmp_type = EntityType::PAVEMENT;
	else if (str == "person")
		tmp_type = EntityType::PERSON;

	return tmp_type;
}

Entity* make_entity_from_type(EntityType type)
{
	Entity *tmp_en = nullptr;
	if(type == EntityType::CAR)
		tmp_en = new Car;
	else if(type == EntityType::JUNCTION)
		tmp_en = new Junction;
	else if(type == EntityType::PAVEMENT)
		tmp_en = new Pavement;
	else if(type == EntityType::PERSON)
		tmp_en = new Person;
	else if(type == EntityType::ROAD)
		tmp_en = new Road;
	else if(type == EntityType::ROAD_4_JUNC)
		tmp_en = new Road_4_Junc;
	else if(type == EntityType::RECT_H)
		tmp_en = new Rect_H;
	else if(type == EntityType::RECT_V)
		tmp_en = new Rect_V;
	else if(type == EntityType::TRAFFIC_LIGHT)
		tmp_en = new TrafficLight;
	else if(type == EntityType::BUS_STAND)
		tmp_en = new BusStand;
	else if(type == EntityType::BUS)
		tmp_en = new Bus;

	return tmp_en;
}

bool if_string_is_digit(std::string str)
{
	bool is_number = true;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			is_number = false;
	}

	return is_number;
}

bool is_en_type_a_vehicle(EntityType type)
{
	bool is_vehicle = false;
	switch(type)
	{
		case EntityType::CAR:
		case EntityType::BUS:
			is_vehicle = true;
			break;
		default:
			is_vehicle = false;
	}

	return is_vehicle;
}

Entity* get_pavement_next_to_road_with_bst_id(int id, Graph *gp)
{
	Entity *tmp_en = nullptr;

	Node* bst_en = gp->get_node_with_id(id);

	for(unsigned int i = 0; i < bst_en->adjacent_nodes.size(); i++)
	{
		if(bst_en->adjacent_nodes[i]->type == EntityType::PAVEMENT)
		{
			Node *to_check_node = gp->get_node_with_id(bst_en->adjacent_nodes[i]->id);

			for(unsigned int j = 0; j < to_check_node->adjacent_nodes.size(); j++)
			{
				if(to_check_node->adjacent_nodes[j]->type == EntityType::ROAD)
				{
					tmp_en = to_check_node->en;
					break;
				}
			}
		}
	}

	return tmp_en;
}

Entity* get_entity_next_to_type_with_id(int id, EntityType type, Graph *gp)
{
	Entity *tmp_en = nullptr;

	for(unsigned int i = 0; i < gp->nodes.size(); i++)
	{
		if(gp->nodes[i]->en->id == id)
		{
			for(unsigned int j = 0; j < gp->nodes[i]->adjacent_nodes.size(); j++)
			{
				if(gp->nodes[i]->adjacent_nodes[j]->type == type)
				{
					tmp_en = gp->nodes[i]->adjacent_nodes[j];
					break;
				}
			}
		}
	}

	return tmp_en;
}

std::vector<Entity*> get_all_bus_that_goes_through(int src_bst_id, int des_bst_id, Simulation *sim)
{
	std::vector<Entity*> bus_ens;
	int count = 0;

	/*
	std::cout << "src_bst_id: " << src_bst_id << std::endl;
	std::cout << "des_bst_id: " << des_bst_id << std::endl;
	*/

	for(unsigned int i = 0; i < sim->bus_ens.size(); i++)
	{
		Entity *current_bus = sim->bus_ens[i];
		std::vector<int> bst_ids = ((Bus*)(current_bus))->bus_stands;

		/*
		std::cout << "bus_with id: " << sim->bus_ens[i]->id << " goes to : ";
		for(unsigned int x = 0; x < bst_ids.size(); x++)
		{
			std::cout << bst_ids[x] << " ";
		}
		std::cout << std::endl;
		*/
		
		for(unsigned int j = 0; j < bst_ids.size(); j++)
		{
			Entity *to_check = get_bus_stand_with_id(bst_ids[j], sim);	
			if(src_bst_id == to_check->id)
				count++;
			else if(des_bst_id == to_check->id)
				count++;
		}

		if(count >= 2)
		{
			bus_ens.push_back(current_bus);
			count = 0;
		}
	}

	return bus_ens;
}

int get_occ_index_attached_to_pavement(Entity *pave_en, Graph *gp, VirtualGrid *v_grid)
{
	if(!pave_en || pave_en->type != EntityType::PAVEMENT)
		return -1;

	Entity *road_en = get_entity_next_to_type_with_id(pave_en->id, EntityType::ROAD, gp);

	if(!road_en)
		return -1;

	Direction tmp_dir = Direction::NONE;
	SDL_Point road_pos = { road_en->pos.x, road_en->pos.y };
	SDL_Point pave_pos = { pave_en->pos.x, pave_en->pos.y };

	if(road_en->angle == Angle::ZERO || road_en->angle == Angle::HUN_EIGHTY)
	{
		if(road_pos.x > pave_pos.x)
			tmp_dir = Direction::RIGHT;
		else
			tmp_dir = Direction::LEFT;
	}
	else if(road_en->angle == Angle::NINETY || road_en->angle == Angle::TWO_SEVENTY)
	{
		if(road_pos.y > pave_pos.y)
			tmp_dir = Direction::DOWN;
		else
			tmp_dir = Direction::UP;
	}

	int tmp_in = -1;
	if(tmp_dir == Direction::RIGHT)
		tmp_in = pave_en->occupied_indices[0] + 1;
	else if(tmp_dir == Direction::LEFT)
		tmp_in = pave_en->occupied_indices[0] - 1;
	else if(tmp_dir == Direction::DOWN)
		tmp_in = pave_en->occupied_indices[0] + v_grid->virtual_num_cols;
	else if(tmp_dir == Direction::UP)
		tmp_in = pave_en->occupied_indices[0] - v_grid->virtual_num_cols;

	return tmp_in;
}

void update_current_stand_for_bus(Entity *bus_en, const std::vector<Entity*> &bus_stands, Graph *gp, VirtualGrid *v_grid)
{
	if(!bus_en || bus_en->type != EntityType::BUS)
		return;

	int index = bus_en->occupied_indices[0];

	((Bus*)(bus_en))->current_bus_stand = nullptr;
	for(unsigned int i = 0; i < bus_stands.size(); i++) {
		Entity *pave_en = get_entity_next_to_type_with_id(bus_stands[i]->id, EntityType::PAVEMENT, gp);
		int tmp_in = get_occ_index_attached_to_pavement(pave_en, gp, v_grid);
		if(index == tmp_in) {
			((Bus*)(bus_en))->current_bus_stand = bus_stands[i];
			break;
		}
	}
}

Bus_Stops_Path generate_bus_path(Entity *e, EntitySystem *e_sys, Graph *gp, VirtualGrid *v_grid, Camera *cam)
{
	/* First we get pavments next to the bus_stands */
	/* Second get the road next to pavement */
	/* Store all the roads in a vector */
	/* src_index is occ_index of 1st index and des_index is occ_index
	 * of next road */
	/* Append the path to the existing tmp_path<vec> */

	std::vector<int> bus_stand_ids = ((Bus*)(e))->bus_stands;

	std::vector<Entity*> pavements;
	std::vector<int> occ_indices;
	for(unsigned int i = 0; i < bus_stand_ids.size(); i++)
	{
		Entity *tmp_en = get_pavement_next_to_road_with_bst_id(bus_stand_ids[i], gp);

		int tmp_in = get_occ_index_attached_to_pavement(tmp_en, gp, v_grid);
		occ_indices.push_back(tmp_in);
	}

	gp->src_clicked = true;
	gp->des_clicked = true;

	Bus_Stops_Path bst;

	// Computing path from bus'es current_position to first src_index
	// . Otherwise the bus won't move !!!
	if(occ_indices.size() > 0) {
		gp->car_src_index = e->occupied_indices[0];
		gp->car_des_index = occ_indices[0];

		std::vector<SDL_Point> pt = gp->compute_shortest_path_bw(v_grid, cam);
		for(unsigned int i = 0; i < pt.size(); i++) {
			bst.path.push_back(pt[i]);
		}
	}

	for(unsigned int i = 0; i < occ_indices.size() - 1; i++)
	{
		gp->car_src_index = occ_indices[i];
		gp->car_des_index = occ_indices[i + 1];

		std::vector<SDL_Point> pt = gp->compute_shortest_path_bw(v_grid, cam);

		bst.stop_indices.push_back(gp->car_src_index);

		if(i == occ_indices.size() - 2)
			bst.stop_indices.push_back(gp->car_des_index);

		for(unsigned int j = 0; j < pt.size(); j++)
		{
			bst.path.push_back(pt[j]);
		}
	}

	/* Note: A cycle has to be formed otherwise it won't work */
	/* Note: A cycle has to be formed otherwise it won't work */
	/* Note: A cycle has to be formed otherwise it won't work */
	/* Note: A cycle has to be formed otherwise it won't work */
	/* Note: A cycle has to be formed otherwise it won't work */

	/* BusStand clicks have to end in the same stand the click
	 * first started in */
	SDL_Point to_check_point = bst.path[0];
	for(unsigned int i = 1; i < bst.path.size(); i++) {
		if(bst.path[i].x == to_check_point.x && bst.path[i].y == to_check_point.y) {
			bst.path.erase(bst.path.begin() + i, bst.path.end());
			break;
		}
	}

	std::cout << "(After Cycle) Indices for id: " << e->id << " are: ";
	for(unsigned int i = 0; i < bst.path.size(); i++) {
		int in = v_grid->get_index(bst.path[i].x, bst.path[i].y);

		std::cout << in << " ";
	}
	std::cout << std::endl;


	/*
		std::cout << "Stops are: ";
		for(unsigned int i = 0; i < bst.stop_indices.size(); i++)
		{
		std::cout << bst.stop_indices[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "full path is: ";
		for(unsigned int i = 0; i < bst.path.size(); i++)
		{
		std::cout << "(" << bst.path[i].x << "," << bst.path[i].y << ") ";
		}
		std::cout << std::endl;
	*/

	return bst;
}

bool is_coo_within_rect(int x, int y, const SDL_Rect &rect)
{
	if (x > rect.x && x < rect.x + rect.w &&
			y > rect.y && y < rect.y + rect.h)
		return true;

	return false;
}

/* tmp */
void print_angle(Angle ang)
{
	std::string req_ang_string;
	if (ang == Angle::ZERO)
		req_ang_string = "ZERO";
	else if (ang == Angle::NINETY)
		req_ang_string = "NINETY";
	else if (ang == Angle::HUN_EIGHTY)
		req_ang_string = "HUN_EIGHTY";
	else if (ang == Angle::TWO_SEVENTY)
		req_ang_string = "TWO_SEVENTY";

	std::cout << "Angle: " << req_ang_string;
}

void print_entity_type(EntityType type)
{
	std::string req_str;
	if (type == EntityType::NONE)
		req_str = "NONE";
	else if (type == EntityType::ROAD)
		req_str = "ROAD";
	else if (type == EntityType::ROAD_4_JUNC)
		req_str = "ROAD_4_JUNC";
	else if (type == EntityType::CAR)
		req_str = "CAR";
	else if (type == EntityType::BUS)
		req_str = "BUS";
	else if (type == EntityType::JUNCTION)
		req_str = "JUNCTION";
	else if (type == EntityType::PAVEMENT)
		req_str = "PAVEMENT";
	else if (type == EntityType::RECT_H)
		req_str = "RECT_H";
	else if (type == EntityType::RECT_V)
		req_str = "RECT_V";
	else if (type == EntityType::TRAFFIC_LIGHT)
		req_str = "TRAFFIC_LIGHT";
	else if (type == EntityType::BUS_STAND)
		req_str = "BUS_STAND";
	else if (type == EntityType::PERSON)
		req_str = "PERSON";

	std::cout << "entity_type: " << req_str;
}

void print_direction(Direction dir)
{
	std::string str_dir;
	if (dir == Direction::NONE)
		str_dir = "NONE";
	else if (dir == Direction::LEFT)
		str_dir = "LEFT";
	else if (dir == Direction::RIGHT)
		str_dir = "RIGHT";
	else if (dir == Direction::UP)
		str_dir = "UP";
	else if (dir == Direction::DOWN)
		str_dir = "DOWN";

	std::cout << "Direction is: " << str_dir;
}

void print_rect(SDL_Rect *rect)
{
	std::cout << "rect: " << rect->x << " " << rect->y << " " << rect->w << " " << rect->h << std::endl;
}
/* tmp */


void save_entities(EntitySystem *en_sys, std::string f_name, Camera *cam)
{
	/* IMP: First write the non-movable entities, because the movable entities has
	 * to be rendered on top on the non-movable entities, otherwise it can't
	 * be seen.
	 */

	std::ofstream write;
	write.open(f_name.c_str(), std::ios::binary);

	if (!write)
	{
		std::cout << "Error opening: " << f_name << " in save_entities()" << std::endl;
		return;
	}

	std::vector<Entity*> nm_ens;
	std::vector<Entity*> m_ens;
	for(unsigned int i = 0; i < en_sys->entities.size(); i++)
	{
		if(is_en_type_a_vehicle(en_sys->entities[i]->type))
			m_ens.push_back(en_sys->entities[i]);
		else
			nm_ens.push_back(en_sys->entities[i]);
	}

	int num_nm_ens = nm_ens.size();
	int num_m_ens = m_ens.size();

	std::cout << "Saving....." << std::endl;
	std::cout << "num_nm_ens: " << num_nm_ens << std::endl;
	std::cout << "num_m_ens: " << num_m_ens << std::endl;

	// Writing ....
	write.write((char*)(&num_m_ens), sizeof(int));
	write.write((char*)(&num_nm_ens), sizeof(int));

	// non-movable entities
	for(unsigned int i = 0; i < nm_ens.size(); i++)
	{
		// id
		int tmp_id = nm_ens[i]->id;
		write.write((char*)(&tmp_id), sizeof(int));

		// occ_index[0]
		int tmp_index = nm_ens[i]->occupied_indices[0];
		write.write((char*)(&tmp_index), sizeof(int));

		// is_del
		write.write((char*)(&nm_ens[i]->is_deletable), sizeof(bool));

		// angle
		write.write((char*)(&nm_ens[i]->angle), sizeof(Angle));

		// type
		write.write((char*)(&nm_ens[i]->type), sizeof(EntityType));

		/* Note: include traffic_light: time_delay, pos.x, pos.y */
		if(nm_ens[i]->type == EntityType::TRAFFIC_LIGHT)
		{
			int tmp_delay_time = ((TrafficLight*)(nm_ens[i]))->time_delay;
			int tmp_junc_id = ((TrafficLight*)(nm_ens[i]))->junc_id;
			int tmp_tl_x = nm_ens[i]->pos.x + cam->pos.x;
			int tmp_tl_y = nm_ens[i]->pos.y + cam->pos.y;

			write.write((char*)(&tmp_delay_time), sizeof(int));
			write.write((char*)(&tmp_tl_x), sizeof(int));
			write.write((char*)(&tmp_tl_y), sizeof(int));
			write.write((char*)(&tmp_junc_id), sizeof(int));
		}

		else if(nm_ens[i]->type == EntityType::PERSON)
		{
			Person *tmp_person = (Person*)(nm_ens[i]);

			int tmp_image_index = tmp_person->image_index;
			write.write((char*)(&tmp_image_index), sizeof(int));

			int tmp_bus_stand_id = tmp_person->current_bus_stand_id;
			write.write((char*)(&tmp_bus_stand_id), sizeof(int));

			int tmp_des_bus_stand_id = tmp_person->des_bus_stand_id;
			write.write((char*)(&tmp_des_bus_stand_id), sizeof(int));

			std::vector<int> tmp_bordable_bus_ids = tmp_person->bordable_bus_ids;
			int tmp_num_bordable_bus_ids = tmp_bordable_bus_ids.size();

			write.write((char*)(&tmp_num_bordable_bus_ids), sizeof(int));
			for (unsigned int x = 0; x < tmp_bordable_bus_ids.size(); x++)
				write.write((char*)(&tmp_bordable_bus_ids[x]), sizeof(int));
		}
	}

	// movable entities
	for (unsigned int i = 0; i < m_ens.size(); i++)
	{
		// id
		int tmp_id = m_ens[i]->id;
		write.write((char*)(&tmp_id), sizeof(int));

		// occ_index[0]
		int tmp_index = m_ens[i]->occupied_indices[0];
		write.write((char*)(&tmp_index), sizeof(int));

		// is_del
		write.write((char*)(&m_ens[i]->is_deletable), sizeof(bool));

		// angle
		write.write((char*)(&m_ens[i]->angle), sizeof(Angle));

		// type
		write.write((char*)(&m_ens[i]->type), sizeof(EntityType));

		if (m_ens[i]->type == EntityType::CAR)
		{
			// src_index
			int tmp_src_index = ((Car*)(m_ens[i]))->src_index;
			write.write((char*)(&tmp_src_index), sizeof(int));

			// des_index
			int tmp_des_index = ((Car*)(m_ens[i]))->des_index;
			write.write((char*)(&tmp_des_index), sizeof(int));

			// car_image_index
			int tmp_car_image_index = ((Car*)(m_ens[i]))->vehicle_image_index;
			write.write((char*)(&tmp_car_image_index), sizeof(int));
		}

		else if (m_ens[i]->type == EntityType::BUS)
		{
			// Number of bus_stands
			int num_busstands = ((Bus*)(m_ens[i]))->bus_stands.size();
			write.write((char*)(&num_busstands), sizeof(int));

			// bus_stands
			std::vector<int> tmp_vec = ((Bus*)(m_ens[i]))->bus_stands;
			for (unsigned int x = 0; x < tmp_vec.size(); x++)
			{
				int tmp_val = tmp_vec[x];
				write.write((char*)(&tmp_val), sizeof(int));
			}
		}
	}

	std::cout << "Successfully saved: " << f_name << std::endl;
	write.close();
}

void load_entities(EntitySystem *en_sys, std::string f_name, Grid *grid, VirtualGrid *v_grid, Camera *cam, Graph *gp, Simulation *sim)
{
	/* IMP: First write the non-movable entities, because the movable entities has
	 * to be rendered on top on the non-movable entities, otherwise it can't
	 * be seen
	 */

	std::ifstream read;
	read.open(f_name.c_str(), std::ios::binary);

	if (!read)
	{
		std::cout << "Error opening: " << f_name << " in load entities()" << std::endl;
		return;
	}

	// clearing the map
	en_sys->clear_all_entities(v_grid);
	// clearing the map

	unsigned int num_m_ens = 0, num_nm_ens = 0;

	read.read((char*)(&num_m_ens), sizeof(int));
	read.read((char*)(&num_nm_ens), sizeof(int));

	std::cout << "Loading....." << std::endl;
	std::cout << "num_nm_ens: " << num_nm_ens << std::endl;
	std::cout << "num_m_ens: " << num_m_ens << std::endl;

	int count = 0;
	// reading m_ens
	for (unsigned int i = 0; i < num_m_ens + num_nm_ens; i++)
	{
		int tmp_occupied_v_grid_index = 0;
		bool is_del = true;
		int tmp_id = -1;
		Angle angle;
		EntityType type = EntityType::NONE;
		int tmp_src_index = 0, tmp_des_index = 0;
		int tmp_vehicle_image_index = -1;
		int tmp_tl_delay_time = -1;
		int tmp_tl_pos_x = -1, tmp_tl_pos_y = -1;
		int tmp_junc_id = -1;
		std::vector<int> tmp_busstands;
		int tmp_image_index = -1;
		int tmp_ps_bus_stand_id = -1;
		int tmp_ps_des_bus_stand_id = -1;
		std::vector<int> tmp_bordable_bus_ids;

		read.read((char *)(&tmp_id), sizeof(int));
		read.read((char *)(&tmp_occupied_v_grid_index), sizeof(int));
		read.read((char *)(&is_del), sizeof(bool));
		read.read((char *)(&angle), sizeof(Angle));
		read.read((char *)(&type), sizeof(EntityType));

		if (type == EntityType::CAR)
		{
			read.read((char *)(&tmp_src_index), sizeof(int));
			read.read((char *)(&tmp_des_index), sizeof(int));
			read.read((char *)(&tmp_vehicle_image_index), sizeof(int));
		}
		else if (type == EntityType::BUS)
		{
			int tmp_num_busstands = -1;
			read.read((char *)(&tmp_num_busstands), sizeof(int));

			for (int x = 0; x < tmp_num_busstands; x++)
			{
				int tmp_bus_stand_id = -1;
				read.read((char *)(&tmp_bus_stand_id), sizeof(int));
				tmp_busstands.push_back(tmp_bus_stand_id);
			}
		}
		else if (type == EntityType::TRAFFIC_LIGHT)
		{
			read.read((char *)(&tmp_tl_delay_time), sizeof(int));
			read.read((char *)(&tmp_tl_pos_x), sizeof(int));
			read.read((char *)(&tmp_tl_pos_y), sizeof(int));
			read.read((char *)(&tmp_junc_id), sizeof(int));
		}

		else if (type == EntityType::PERSON)
		{
			read.read((char *)(&tmp_image_index), sizeof(int));
			read.read((char *)(&tmp_ps_bus_stand_id), sizeof(int));
			read.read((char *)(&tmp_ps_des_bus_stand_id), sizeof(int));

			unsigned int tmp_num_bus_ids = 0;
			read.read((char *)(&tmp_num_bus_ids), sizeof(int));
			for (unsigned int x = 0; x < tmp_num_bus_ids; x++)
			{
				int tmp_id = -1;
				read.read((char *)(&tmp_id), sizeof(int));
				tmp_bordable_bus_ids.push_back(tmp_id);
			}
		}

		SDL_Point tmp_pos = v_grid->get_pos_from_index(tmp_occupied_v_grid_index);

		// Has to be done before bounding the position to grid dims
		int bef_bound_x = tmp_pos.x;
		int bef_bound_y = tmp_pos.y;
		if (type == EntityType::TRAFFIC_LIGHT)
		{
			bef_bound_x = tmp_tl_pos_x;
			bef_bound_y = tmp_tl_pos_y;
		}
		// Has to be done before bounding the position to grid dims

		tmp_pos = v_grid->bound_pos_to_grid_dims(grid, tmp_pos.x + 2, tmp_pos.y + 2);

		if (type == EntityType::TRAFFIC_LIGHT)
			en_sys->load_entity(type, tmp_pos.x - grid->per_width / 2, tmp_pos.y - grid->per_height / 2, grid, v_grid, cam);
		else
			en_sys->load_entity(type, tmp_pos.x, tmp_pos.y, grid, v_grid, cam);

		// Has to be done before bounding the position to grid dims
		en_sys->entities[count]->pos.x = bef_bound_x;
		en_sys->entities[count]->pos.y = bef_bound_y;
		v_grid->set_filled(bef_bound_x + 2, bef_bound_y + 2, en_sys->entities[count]->num_rows_cols.x, en_sys->entities[count]->num_rows_cols.y);
		en_sys->entities[count]->occupied_indices = v_grid->get_indices(bef_bound_x + 2, bef_bound_y + 2, en_sys->entities[count]->num_rows_cols.x, en_sys->entities[count]->num_rows_cols.y);
		// Has to be done before bounding the position to grid dims

		en_sys->entities[count]->id = tmp_id;
		en_sys->entities[count]->angle = angle;
		en_sys->entities[count]->is_deletable = is_del;

		if (type == EntityType::CAR)
		{
			((Car*)(en_sys->entities[count]))->src_index = tmp_src_index;
			((Car*)(en_sys->entities[count]))->des_index = tmp_des_index;
			((Car*)(en_sys->entities[count]))->vehicle_image_index = tmp_vehicle_image_index;
		}
		else if (type == EntityType::BUS)
		{
			((Bus*)(en_sys->entities[count]))->bus_stands = tmp_busstands;
		}
		else if (type == EntityType::TRAFFIC_LIGHT)
		{
			((TrafficLight*)(en_sys->entities[count]))->time_delay = tmp_tl_delay_time;
			en_sys->entities[count]->pos.x = tmp_tl_pos_x;
			en_sys->entities[count]->pos.y = tmp_tl_pos_y;
			((TrafficLight*)(en_sys->entities[count]))->junc_id = tmp_junc_id;
		}
		else if (type == EntityType::PERSON)
		{
			((Person*)(en_sys->entities[count]))->image_index = tmp_image_index;
			((Person*)(en_sys->entities[count]))->current_bus_stand_id = tmp_ps_bus_stand_id;
			((Person*)(en_sys->entities[count]))->des_bus_stand_id = tmp_ps_des_bus_stand_id;
			((Person*)(en_sys->entities[count]))->bordable_bus_ids = tmp_bordable_bus_ids;
		}

		init_en_occu_indices(en_sys->entities[count]->occ_conn, en_sys->entities[count]->occupied_indices, type, angle);
		count++;

		// Note: check for updates in add_entity() func in entity.cpp
	}

	init(sim);
	read.close();
}

bool if_connected(EntitySystem *en_sys, VirtualGrid *v_grid, Entity *e1, Entity *e2)
{
	if (!e1 || !e2)
		return false;

	bool is_con = false;

	if (if_road_connected_to_road(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_road_connected_to_road4_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_road4_junc_connected_to_road4_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_junc_connected_to_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_junc_connected_to_road(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_junc_connected_to_road4_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_road_connected_to_pavement(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_pavement_connected_to_pavement(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_recth_connected_to_road(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_recth_connected_to_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_recth_connected_to_road4junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_rectv_connected_to_road(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_rectv_connected_to_junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_rectv_connected_to_road4junc(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}
	else if (if_busstand_connected_to_pavement(en_sys, e1, e2, v_grid))
	{
		is_con = true;
	}

	return is_con;
}

bool if_e1_surrounding_e2(EntitySystem *e_sys, VirtualGrid *v_grid, Entity *e1, Entity *e2)
{
	bool is_surrounding = false;
	std::vector<Entity *> tmp_ens = e_sys->get_entities_surrounding_entity(e1, v_grid);

	for (unsigned int i = 0; i < tmp_ens.size(); i++)
	{
		if (tmp_ens[i]->id == e2->id)
		{
			is_surrounding = true;
			break;
		}
	}

	return is_surrounding;
}

bool if_road_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	if (e1->type != EntityType::ROAD || e2->type != EntityType::ROAD)
		return false;


	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	bool is_con = false;
	Direction tmp_dir = get_direction_of_entities(e1, e2);

	if (tmp_dir == Direction::RIGHT || tmp_dir == Direction::LEFT)
	{
		if (if_angle_is_left_or_right(e1->angle, e2->angle))
			is_con = true;
	}
	else if (tmp_dir == Direction::DOWN || tmp_dir == Direction::UP)
	{
		if (if_angle_is_up_or_down(e1->angle, e2->angle))
			is_con = true;
	}

	return is_con;
}

bool if_road_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *road_en = nullptr;
	Entity *junc_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::ROAD, EntityType::ROAD_4_JUNC, road_en, junc_en))
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	bool is_con = false;

	Direction tmp_dir = Direction::NONE;
	tmp_dir = get_direction_of_entities(road_en, junc_en);

	if (tmp_dir == Direction::LEFT || tmp_dir == Direction::RIGHT)
	{
		if (road_en->angle == Angle::NINETY || road_en->angle == Angle::TWO_SEVENTY)
			is_con = true;
	}
	else if (tmp_dir == Direction::UP || tmp_dir == Direction::DOWN)
	{
		if (road_en->angle == Angle::ZERO || road_en->angle == Angle::HUN_EIGHTY)
			is_con = true;
	}

	return is_con;
}

bool if_road4_junc_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	if (e1->type != EntityType::ROAD_4_JUNC || e2->type != EntityType::ROAD_4_JUNC)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	bool is_con = false;

	Direction tmp_dir = get_direction_of_entities(e1, e2);
	if (tmp_dir == Direction::LEFT || tmp_dir == Direction::RIGHT)
	{
		if (if_angle_is_left_or_right(e1->angle, e2->angle))
			is_con = true;
	}

	else if (tmp_dir == Direction::UP || tmp_dir == Direction::DOWN)
	{
		if (if_angle_is_up_or_down(e1->angle, e2->angle))
			is_con = true;
	}

	return is_con;
}

bool if_junc_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *road_en = nullptr;
	Entity *junc_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::JUNCTION, EntityType::ROAD, junc_en, road_en))
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	bool is_con = false;

	Direction tmp_dir = get_direction_of_entities(road_en, junc_en);
	if (tmp_dir == Direction::LEFT || tmp_dir == Direction::RIGHT)
	{
		if (road_en->angle == Angle::TWO_SEVENTY || road_en->angle == Angle::NINETY)
			is_con = true;
	}
	else if (tmp_dir == Direction::UP || tmp_dir == Direction::DOWN)
	{
		if (road_en->angle == Angle::ZERO || road_en->angle == Angle::HUN_EIGHTY)
			is_con = true;
	}

	return is_con;
}

bool if_junc_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	if (e1->type != EntityType::JUNCTION || e2->type != EntityType::JUNCTION)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_busstand_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *pavement_en = nullptr;
	Entity *busstand_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::BUS_STAND, EntityType::PAVEMENT, busstand_en, pavement_en))
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_junc_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *junc_en = nullptr;
	Entity *junc_4_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::JUNCTION, EntityType::ROAD_4_JUNC, junc_en, junc_4_en))
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	bool is_con = false;

	Direction tmp_dir = Direction::NONE;
	tmp_dir = get_direction_of_entities(junc_en, junc_4_en);

	if (tmp_dir != Direction::NONE)
		is_con = true;

	return is_con;
}

bool if_road_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *road_en = nullptr;
	Entity *pavement_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::ROAD, EntityType::PAVEMENT, road_en, pavement_en))
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_pavement_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	if (e1->type != EntityType::PAVEMENT || e2->type != EntityType::PAVEMENT)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_recth_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *road_en = nullptr;
	Entity *rect_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::ROAD, EntityType::RECT_H, road_en, rect_en))
		return false;

	if (road_en->angle == Angle::NINETY || road_en->angle == Angle::TWO_SEVENTY)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_recth_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *rect_en = nullptr;
	Entity *junc_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::RECT_H, EntityType::JUNCTION, rect_en, junc_en))
		return false;

	Direction tmp_dir = get_direction_of_entities(e1, e2);
	if (tmp_dir == Direction::LEFT || tmp_dir == Direction::RIGHT)
	{
		return false;
	}

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_recth_connected_to_road4junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *rect_en = nullptr;
	Entity *junc4_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::RECT_H, EntityType::ROAD_4_JUNC, rect_en, junc4_en))
		return false;

	Direction tmp_dir = get_direction_of_entities(e1, e2);
	if (tmp_dir == Direction::LEFT || tmp_dir == Direction::RIGHT)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_rectv_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *road_en = nullptr;
	Entity *rect_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::ROAD, EntityType::RECT_V, road_en, rect_en))
		return false;

	if (road_en->angle == Angle::ZERO || road_en->angle == Angle::HUN_EIGHTY)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_rectv_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *rect_en = nullptr;
	Entity *junc_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::RECT_V, EntityType::JUNCTION, rect_en, junc_en))
		return false;

	Direction tmp_dir = get_direction_of_entities(e1, e2);
	if (tmp_dir == Direction::UP || tmp_dir == Direction::DOWN)
	{
		return false;
	}

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_rectv_connected_to_road4junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid)
{
	Entity *rect_en = nullptr;
	Entity *junc4_en = nullptr;

	if (!check_and_assign_types(e1, e2, EntityType::RECT_V, EntityType::ROAD_4_JUNC, rect_en, junc4_en))
		return false;

	Direction tmp_dir = get_direction_of_entities(e1, e2);
	if (tmp_dir == Direction::UP || tmp_dir == Direction::DOWN)
		return false;

	if (!if_e1_surrounding_e2(e_sys, v_grid, e1, e2))
		return false;

	return true;
}

bool if_angle_is_left_or_right(Angle a1, Angle a2)
{
	if ((a1 == Angle::NINETY || a1 == Angle::TWO_SEVENTY) &&
		(a2 == Angle::NINETY || a2 == Angle::TWO_SEVENTY))
		return true;

	return false;
}

bool if_angle_is_up_or_down(Angle a1, Angle a2)
{
	if ((a1 == Angle::ZERO || a1 == Angle::HUN_EIGHTY) &&
		(a2 == Angle::ZERO || a2 == Angle::HUN_EIGHTY))
		return true;

	return false;
}

/* It only gives Direction != NONE if directly above, below, right or left, won't work if entities are not next to each other
*/
Direction get_direction_of_entities(Entity *source_en, Entity *des_en)
{
	SDL_Point source_p{ source_en->pos.x + source_en->pos.w / 2, source_en->pos.y + source_en->pos.h / 2 };
	SDL_Point des_p{ des_en->pos.x + des_en->pos.w / 2, des_en->pos.y + des_en->pos.h / 2 };

	Direction tmp_dir = Direction::NONE;
	int range = 2;

	if (if_within_range(source_p.y, des_p.y, range) && des_p.x > source_p.x) // des_en is right of source_en
		tmp_dir = Direction::RIGHT;
	else if (if_within_range(source_p.y, des_p.y, range) && des_p.x < source_p.x) // des_en is left of source_en
		tmp_dir = Direction::LEFT;
	else if (if_within_range(source_p.x, des_p.x, range) && des_p.y < source_p.y) // des_en is above source_en
		tmp_dir = Direction::UP;
	else if (if_within_range(source_p.x, des_p.x, range) && des_p.y > source_p.y) // des_en is below source_en
		tmp_dir = Direction::DOWN;

	return tmp_dir;
}

bool if_within_range(int i1, int i2, int dev)
{
	bool is_within = false;
	if (i1 >= (i2 - dev) && i1 <= (i2 + dev))
		is_within = true;

	return is_within;
}

std::map<int, std::vector<int>> get_en_internal_connections(EntityType type, Angle angle)
{
	std::map<int, std::vector<int>> tmp_map;

	if (type == EntityType::ROAD)
	{
		if (angle == Angle::ZERO || angle == Angle::HUN_EIGHTY)
		{
			tmp_map[2] = std::vector<int>{ 0 };
			tmp_map[1] = std::vector<int>{ 3 };
		}
		else if (angle == Angle::NINETY || angle == Angle::TWO_SEVENTY)
		{
			tmp_map[0] = std::vector<int>{ 1 };
			tmp_map[3] = std::vector<int>{ 2 };
		}
	}

	else if (type == EntityType::JUNCTION)
	{
		tmp_map[0] = std::vector<int>{ 2, 1 };
		tmp_map[1] = std::vector<int>{ 0, 3 };
		tmp_map[2] = std::vector<int>{ 3, 0 };
		tmp_map[3] = std::vector<int>{ 1, 2 };
	}

	else if (type == EntityType::ROAD_4_JUNC)
	{
		tmp_map[2] = std::vector<int>{ 6 };
		tmp_map[4] = std::vector<int>{ 5 };
		tmp_map[5] = std::vector<int>{ 1, 6 };
		tmp_map[6] = std::vector<int>{ 7, 10 };
		tmp_map[9] = std::vector<int>{ 8, 5 };
		tmp_map[10] = std::vector<int>{ 9, 14 };
		tmp_map[11] = std::vector<int>{ 10 };
		tmp_map[13] = std::vector<int>{ 9 };
	}

	return tmp_map;
}

void init_en_occu_indices(std::map<std::map<int, int>, std::vector<int>> &oc, const std::vector<int> &occupied_indices, EntityType type, Angle angle)
{
	oc.erase(oc.begin(), oc.end());

	/* Note: If type is RECT_V or RECT_H, it has no internal connections
	 * but the std::map<int, int> has to be mapped to an empty vector */
	if (type == EntityType::RECT_H || type == EntityType::RECT_V)
	{
		std::map<int, int> tmp_m_1, tmp_m_2;
		tmp_m_1[0] = occupied_indices[0];
		tmp_m_2[1] = occupied_indices[1];
		oc[tmp_m_1] = std::vector<int>{};
		oc[tmp_m_2] = std::vector<int>{};

		return;
	}

	std::map<int, std::vector<int>> in_con = get_en_internal_connections(type, angle);
	std::map<int, std::vector<int>>::iterator it = in_con.begin();

	for (; it != in_con.end(); ++it)
	{
		int current_in = it->first;
		std::vector<int> cons = it->second;
		std::vector<int> tmp_vec;
		std::map<int, int> tmp_map;

		for (unsigned int j = 0; j < cons.size(); j++)
			tmp_vec.push_back(occupied_indices[cons[j]]);

		tmp_map[current_in] = occupied_indices[current_in];
		oc[tmp_map] = tmp_vec;
	}
}

void connect_both_road_and_road(Entity *e1, Entity *e2)
{
	// connecting e1 with e2
	Direction dir = get_direction_of_entities(e1, e2);

	if (dir == Direction::UP)
	{
		add_index_to_occ_conn(e1, e2, 0, 2);
		add_index_to_occ_conn(e2, e1, 3, 1);
	}
	else if (dir == Direction::DOWN)
	{
		add_index_to_occ_conn(e2, e1, 0, 2);
		add_index_to_occ_conn(e1, e2, 3, 1);
	}
	else if (dir == Direction::RIGHT)
	{
		add_index_to_occ_conn(e1, e2, 1, 0);
		add_index_to_occ_conn(e2, e1, 2, 3);
	}
	else if (dir == Direction::LEFT)
	{
		add_index_to_occ_conn(e2, e1, 1, 0);
		add_index_to_occ_conn(e1, e2, 2, 3);
	}
}

void connect_both_road_and_junction(Entity *e1, Entity *e2)
{
	// Since road_to_road and junction_to_road are the same
	// we are using connect_both_road_and_road()

	connect_both_road_and_road(e1, e2);
}

void connect_both_road_and_road4junc(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);

	if (dir == Direction::DOWN) // road is down
	{
		add_index_to_occ_conn(e2, e1, 0, 13);
		add_index_to_occ_conn(e1, e2, 14, 1);
	}
	else if (dir == Direction::UP)
	{
		add_index_to_occ_conn(e1, e2, 1, 2);
		add_index_to_occ_conn(e2, e1, 3, 2);
	}
	else if (dir == Direction::RIGHT)
	{
		add_index_to_occ_conn(e1, e2, 7, 0);
		add_index_to_occ_conn(e2, e1, 2, 11);
	}
	else if (dir == Direction::LEFT)
	{
		add_index_to_occ_conn(e1, e2, 8, 3);
		add_index_to_occ_conn(e2, e1, 1, 4);
	}
}

void connect_both_junction_and_road4junc(Entity *e1, Entity *e2)
{
	// Since road are same when connecting
	// we use connect_both_road_and_road4junc

	connect_both_road_and_road4junc(e1, e2);
}

void connect_both_junction_and_junction(Entity *e1, Entity *e2)
{
	// Since road_to_road and junction_to_junction are the same
	// we are using connect_both_road_and_road()

	connect_both_road_and_road(e1, e2);
}

void connect_both_road4junc_and_road4junc(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);

	if (dir == Direction::DOWN) // e2 is down
	{
		add_index_to_occ_conn(e2, e1, 1, 13);
		add_index_to_occ_conn(e1, e2, 14, 2);
	}
	else if (dir == Direction::UP) // e2 is up
	{
		add_index_to_occ_conn(e1, e2, 1, 13);
		add_index_to_occ_conn(e2, e1, 14, 2);
	}
	else if (dir == Direction::RIGHT) // e2 is right
	{
		add_index_to_occ_conn(e1, e2, 7, 4);
		add_index_to_occ_conn(e2, e1, 8, 11);
	}
	else if (dir == Direction::LEFT) // e2 is left
	{
		add_index_to_occ_conn(e2, e1, 7, 4);
		add_index_to_occ_conn(e1, e2, 8, 11);
	}
}

void connect_both_recth_and_road(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);
	// e1: rect_h, e2: road

	if (dir == Direction::DOWN)
	{
		add_index_to_occ_conn(e2, e1, 0, 0);
		add_index_to_occ_conn(e1, e2, 1, 1);
	}
	else if (dir == Direction::UP)
	{
		add_index_to_occ_conn(e1, e2, 0, 2);
		add_index_to_occ_conn(e2, e1, 3, 1);
	}
}

void connect_both_recth_and_junction(Entity *e1, Entity *e2)
{
	connect_both_recth_and_road(e1, e2);
}

void connect_both_recth_and_road4junc(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);
	// e1: rect_h, e2: road_4_junc

	if (dir == Direction::DOWN)
	{
		add_index_to_occ_conn(e1, e2, 1, 2);
		add_index_to_occ_conn(e2, e1, 1, 0);
	}
	else if (dir == Direction::UP)
	{
		add_index_to_occ_conn(e1, e2, 0, 13);
		add_index_to_occ_conn(e2, e1, 14, 1);
	}
}

void connect_both_rectv_and_road(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);
	// e1: rect_v, e2: road

	if (dir == Direction::RIGHT) // road to right
	{
		add_index_to_occ_conn(e1, e2, 0, 0);
		add_index_to_occ_conn(e2, e1, 2, 1);
	}
	else if (dir == Direction::LEFT) // road to left
	{
		add_index_to_occ_conn(e2, e1, 1, 0);
		add_index_to_occ_conn(e1, e2, 1, 3);
	}
}

void connect_both_rectv_and_junction(Entity *e1, Entity *e2)
{
	connect_both_rectv_and_road(e1, e2);
}

void connect_both_rectv_and_road4junc(Entity *e1, Entity *e2)
{
	Direction dir = get_direction_of_entities(e1, e2);
	// e1: rect_v, e2: road_4_junc

	if (dir == Direction::RIGHT) // road_4_junc is right
	{
		add_index_to_occ_conn(e2, e1, 8, 1);
		add_index_to_occ_conn(e1, e2, 0, 4);
	}
	else if (dir == Direction::LEFT) // road_4_junc is left
	{
		add_index_to_occ_conn(e2, e1, 7, 0);
		add_index_to_occ_conn(e1, e2, 1, 11);
	}
}

// Seems to be working
void add_index_to_occ_conn(Entity *from_en, Entity *to_en, int from_in, int to_in)
{
	std::map<int, int> tmp_map_1 = from_en->get_mapped_occ_en();
	std::map<int, int> tmp_map_2 = to_en->get_mapped_occ_en();
	std::map<int, int>::iterator it_1, it_2;

	it_1 = tmp_map_1.find(from_in);
	it_2 = tmp_map_2.find(to_in);

	std::map<std::map<int, int>, std::vector<int>>::iterator tmp_it = from_en->occ_conn.begin();

	for (; tmp_it != from_en->occ_conn.end(); ++tmp_it)
	{
		std::map<int, int> in_map = tmp_it->first;
		std::map<int, int>::iterator in_it = in_map.begin();

		if (in_it->first == from_in)
		{
			std::vector<int> tmp_vec = tmp_it->second;

			// Checking if value already present
			bool already_present = false;
			for (unsigned int j = 0; j < tmp_vec.size(); j++)
			{
				if (tmp_vec[j] == it_2->second)
					already_present = true;
			}
			if (!already_present)
			{
				tmp_vec.push_back(it_2->second);
				tmp_it->second = tmp_vec;
			}
			break;
		}
	}

	if (tmp_it == from_en->occ_conn.end())
	{
		std::map<int, int> tmp_map;
		tmp_map[it_1->first] = it_1->second;
		from_en->occ_conn[tmp_map] = std::vector<int>{ it_2->second };
	}
}

void show_all_occ_en(Entity *en)
{
	std::map<std::map<int, int>, std::vector<int>>::iterator tmp_it = en->occ_conn.begin();

	int i = 0;

	std::cout << "en->id: " << en->id << std::endl;

	for (; tmp_it != en->occ_conn.end(); ++tmp_it)
	{
		std::map<int, int> tmp_m = tmp_it->first;
		std::vector<int> tmp_vec = tmp_it->second;

		std::map<int, int>::iterator tmp_it_2 = tmp_m.begin();

		std::cout << tmp_it_2->second << " is connected to: ";
		for (unsigned int j = 0; j < tmp_vec.size(); j++)
			std::cout << tmp_vec[j] << " ";
		std::cout << std::endl;

		i++;
	}

	std::cout << "------------" << std::endl;
}

bool check_and_assign_types(Entity *e1, Entity *e2, EntityType t1, EntityType t2, Entity* &a1, Entity* &a2)
{
	if (e1->type == t1 && e2->type == t2)
	{
		a1 = e1;
		a2 = e2;
	}
	else if (e1->type == t2 && e2->type == t1)
	{
		a1 = e2;
		a2 = e1;
	}

	if (!a1 || !a2)
		return false;

	return true;
}

/*
	std::vector<Entity*> get_pavements_next_to_busstand(int bs_id, Graph *gp, EntitySystem *e_sys)
	{
	std::vector<Entity*> pave_ens;
	Entity *bus_en = e_sys->get_en_with_id(bs_id);
	if(bus_en && bus_en->type == EntityType::BUS_STAND)
	{
	Node *bus_node = gp->get_node_with_id(bs_id);
	if(bus_node)
	{
	for(unsigned int i = 0; i < bus_node->adjacent_nodes.size(); i++)
	{
	Entity *c_en = bus_node->adjacent_nodes[i];
	if(c_en->type == EntityType::PAVEMENT)
	{
	std::vector<Entity*> tmp_vec = get_pavements_next_to_pavement(c_en, gp);
	for(unsigned int j = 0; j < tmp_vec.size(); j++)
	pave_ens.push_back(tmp_vec[j]);
	}
	}
	}
	}

	return pave_ens;
	}
	*/

std::vector<Entity*> get_pavements_next_to_pavement(Entity *pave_en, Graph *gp)
{
	std::vector<Entity*> pave_ens;
	if (pave_en && pave_en->type == EntityType::PAVEMENT)
	{
		Node *tmp_node = gp->get_node_with_id(pave_en->id);
		if (tmp_node)
		{
			for (unsigned int i = 0; i < tmp_node->adjacent_nodes.size(); i++)
			{
				if (tmp_node->adjacent_nodes[i]->type == EntityType::PAVEMENT)
					pave_ens.push_back(tmp_node->adjacent_nodes[i]);
			}
		}
	}

	return pave_ens;
}

void get_all_pavements_rc(std::vector<Entity*> &vec, Entity *start_en, Graph *gp, std::vector<int> &vtd_ids)
{
	std::vector<Entity*> tmp_vec = get_pavements_next_to_pavement(start_en, gp);
	vtd_ids.push_back(start_en->id);
	for (unsigned int i = 0; i < tmp_vec.size(); i++)
	{
		bool already_vtd = false;
		for (unsigned int j = 0; j < vtd_ids.size(); j++)
		{
			if (vtd_ids[j] == tmp_vec[i]->id)
			{
				already_vtd = true;
				break;
			}
		}
		if (!already_vtd)
		{
			bool value_is_in = false;
			for (unsigned int x = 0; x < vec.size(); x++)
			{
				if (tmp_vec[i]->id == vec[x]->id)
				{
					value_is_in = true;
				}
			}
			if (!value_is_in)
			{
				vec.push_back(tmp_vec[i]);
			}

			get_all_pavements_rc(vec, tmp_vec[i], gp, vtd_ids);
		}
	}
}

void set_current_bs_to_person(Entity *ps_en, Graph *gp, EntitySystem *e_sys)
{
	if (!ps_en || ps_en->type != EntityType::PERSON)
		return;

	std::vector<Entity*> all_ens = e_sys->get_all_entities_at_index(ps_en->occupied_indices[0]);
	Entity *start_pave_en = nullptr;
	int bus_stand_id = -1;

	for (unsigned int i = 0; i < all_ens.size(); i++)
	{
		if (all_ens[i]->type == EntityType::PAVEMENT)
		{
			start_pave_en = all_ens[i];
			break;
		}
	}

	if (!start_pave_en)
		return;

	std::vector<Entity*> all_pave_ens = get_pavements_next_to_pavement(start_pave_en, gp);
	std::vector<int> vtd_ids;
	get_all_pavements_rc(all_pave_ens, start_pave_en, gp, vtd_ids);

	/*
		std::cout << "all_pave_ens.size(): " << all_pave_ens.size() << std::endl;
		std::cout << "pave_ids: ";
		for(unsigned int i = 0; i < all_pave_ens.size(); i++)
		{
		std::cout << all_pave_ens[i]->id << " ";
		}
		std::cout << std::endl;
		*/

	for (unsigned int i = 0; i < all_pave_ens.size(); i++)
	{
		Node *tmp_node = gp->get_node_with_id(all_pave_ens[i]->id);
		for (unsigned int j = 0; j < tmp_node->adjacent_nodes.size(); j++)
		{
			if (tmp_node->adjacent_nodes[j]->type == EntityType::BUS_STAND)
			{
				bus_stand_id = tmp_node->adjacent_nodes[j]->id;
				break;
			}
		}
	}

	((Person*)(ps_en))->current_bus_stand_id = bus_stand_id;
	// std::cout << "bus_stand_id: " << bus_stand_id << std::endl;
}
