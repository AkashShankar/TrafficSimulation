#include "pch.h"
#include "entity.h"
#include "grid.h"
#include "camera.h"
#include "graph.h"
#include "utilities.h"

#include <iostream>
#include <vector>

void Entity::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	this->id = id;

	pos.x = x;
	pos.y = y;
}

std::vector<int> Entity::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;
	return tmp_ins;
}

void Entity::draw(SDL_Renderer *ren, SDL_Texture *tex)
{
	float f_angle = 0;
	if(angle == Angle::ZERO)
		f_angle = 0;
	else if(angle == Angle::NINETY)
		f_angle = 90;
	else if(angle == Angle::HUN_EIGHTY)
		f_angle = 180;
	else if(angle == Angle::TWO_SEVENTY)
		f_angle = 270;

	SDL_RenderCopyEx(ren, tex, NULL, &pos, f_angle, NULL, SDL_FLIP_NONE);
}

std::string Entity::get_pos_in_string()
{
	std::string tmp = "x:";
	tmp += std::to_string(pos.x);

	tmp += " y:";
	tmp += std::to_string(pos.y);

	tmp += " w:";
	tmp += std::to_string(pos.w);

	tmp += " h:";
	tmp += std::to_string(pos.h);

	return tmp;
}

void Entity::connect_en(Entity *to_en) {}
int Entity::get_arrow_index() { return -1; };
int Entity::get_tl_color_index() { return -1; };
void Entity::goto_next_traffic_pattern() {}
std::string Entity::get_delaytime_in_string() { return std::string(); }

std::map<int, int> Entity::get_mapped_occ_en()
{
	std::map<int, int> tmp_map;
	for(unsigned int i = 0; i < occupied_indices.size(); i++)
		tmp_map[i] = occupied_indices[i];

	return tmp_map;
}

std::string Entity::get_del_in_string()
{
	std::string str = is_deletable ? "yes" : "no";
	return str;
}

void Entity::goto_next_angle()
{
	if (angle == Angle::ZERO)
		angle = Angle::NINETY;
	else if (angle == Angle::NINETY)
		angle = Angle::HUN_EIGHTY;
	else if (angle == Angle::HUN_EIGHTY)
		angle = Angle::TWO_SEVENTY;
	else if (angle == Angle::TWO_SEVENTY)
		angle = Angle::ZERO;

	init_en_occu_indices(occ_conn, occupied_indices, type, angle);
}

void Entity::goto_previous_angle()
{
	if (angle == Angle::ZERO)
		angle = Angle::TWO_SEVENTY;
	else if (angle == Angle::NINETY)
		angle = Angle::ZERO;
	else if (angle == Angle::HUN_EIGHTY)
		angle = Angle::NINETY;
	else if (angle == Angle::TWO_SEVENTY)
		angle = Angle::HUN_EIGHTY;

	init_en_occu_indices(occ_conn, occupied_indices, type, angle);
}

// Use this only for moving entities ( car, person etc ... )
void Entity::update_indices(Direction dir, VirtualGrid *v_grid, int new_x_pos, int new_y_pos)
{
	// Entity::set_current_indices_to_empty(v_grid);

	std::vector<int> tmp_indices = v_grid->get_indices(new_x_pos, new_y_pos, num_rows_cols.x, num_rows_cols.y);

	for (unsigned int i = 0; i < occupied_indices.size(); i++)
	{
		occupied_indices[i] = tmp_indices[i];
	}

	// v_grid->set_filled(new_x_pos, new_y_pos, num_rows_cols.x, num_rows_cols.y);
}

void Entity::set_current_indices_to_empty(VirtualGrid *v_grid)
{
	for (unsigned int i = 0; i < occupied_indices.size(); i++)
	{
		v_grid->filled_indices[occupied_indices[i]] = false;
	}
}

bool Entity::move(SDL_Point p, VirtualGrid *v_grid, Camera *cam) { return false; };

std::string Entity::get_angle_in_string()
{
	std::string tmp;
	if (angle == Angle::ZERO)
		tmp = "zero";
	else if (angle == Angle::NINETY)
		tmp = "ninety";
	else if (angle == Angle::HUN_EIGHTY)
		tmp = "hundred and eighty";
	else if (angle == Angle::TWO_SEVENTY)
		tmp = "two seventy";

	return tmp;
}

std::string Entity::get_indices_in_string()
{
	std::string tmp;
	for (unsigned int i = 0; i < occupied_indices.size(); i++)
	{
		tmp += std::to_string(occupied_indices[i]);
		tmp += " ";
	}

	return tmp;
}

void EntitySystem::init(SDL_Renderer *ren)
{
	rn = new RandomNumbers;
	rn->init();

	Image *tmp = make_image("Assets\\test_road_2.png", ren);
	en_images[EntityType::ROAD] = std::vector<Image*> { tmp };

	tmp = make_image("Assets\\junction.jpg", ren);
	en_images[EntityType::JUNCTION] = std::vector<Image*> { tmp };

	tmp = make_image("Assets\\road_1.png", ren);
	en_images[EntityType::ROAD_4_JUNC] = std::vector<Image*> { tmp };

	tmp = make_image("Assets\\pavement.png", ren);
	en_images[EntityType::PAVEMENT] = std::vector<Image*> { tmp };

	// people
	std::vector<Image*> tmp_people_images;
	for (int i = 1; i < 21; i++)
	{
		std::string tmp_str = "Assets\\people\\p_";
		tmp_str += std::to_string(i);
		tmp_str += ".png";
		tmp = make_image(tmp_str, ren);
		SDL_SetTextureAlphaMod(tmp->tex, 150);
		tmp_people_images.push_back(tmp);
	}
	en_images[EntityType::PERSON] = tmp_people_images;

	tmp = make_image("Assets\\rect_h.png", ren);
	en_images[EntityType::RECT_H] = std::vector<Image*> { tmp };

	tmp = make_image("Assets\\rect_v.png", ren);
	en_images[EntityType::RECT_V] = std::vector<Image*> { tmp };

	std::vector<Image*> tmp_car_images;
	tmp = make_image("Assets\\orange_car.png", ren);
	tmp_car_images.push_back(tmp);
	tmp = make_image("Assets\\Audi.png", ren);
	tmp_car_images.push_back(tmp);
	tmp = make_image("Assets\\police.png", ren);
	tmp_car_images.push_back(tmp);
	tmp = make_image("Assets\\black_viper.png", ren);
	tmp_car_images.push_back(tmp);
	en_images[EntityType::CAR] = tmp_car_images;

	std::vector<Image*> tmp_traffic_light_images;
	tmp = make_image("Assets\\red_circle.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	tmp = make_image("Assets\\green_circle.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	tmp = make_image("Assets\\arrowDown.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	tmp = make_image("Assets\\arrowLeft.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	tmp = make_image("Assets\\arrowRight.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	tmp = make_image("Assets\\arrowUp.png", ren);
	tmp_traffic_light_images.push_back(tmp);
	en_images[EntityType::TRAFFIC_LIGHT] = tmp_traffic_light_images;

	tmp = make_image("Assets\\stand.png", ren);
	en_images[EntityType::BUS_STAND] = std::vector<Image*> { tmp };

	tmp = make_image("Assets\\bus.png", ren);
	en_images[EntityType::BUS] = std::vector<Image*> { tmp };
}

void EntitySystem::draw(SDL_Renderer *ren)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		int vector_index_1 = -1;
		int vector_index_2 = -1;
		bool is_type_traffic_light = false;

		if (entities[i]->type == EntityType::CAR)
			vector_index_1 = ((Vehicle*)(entities[i]))->vehicle_image_index;
		else if (entities[i]->type == EntityType::PERSON)
		{
			bool is_travelling = ((Person*)(entities[i]))->is_travelling;
			if (!is_travelling)
				vector_index_1 = ((Person*)(entities[i]))->image_index;
		}
		else if (entities[i]->type == EntityType::TRAFFIC_LIGHT)
		{
			is_type_traffic_light = true;
			vector_index_1 = entities[i]->get_tl_color_index();
			vector_index_2 = entities[i]->get_arrow_index();
		}
		else
			vector_index_1 = 0;

		std::vector<Image*> tmp_imgs = en_images[entities[i]->type];
		if (is_type_traffic_light)
		{
			entities[i]->draw(ren, tmp_imgs[vector_index_1]->tex);
			if (vector_index_2 >= 0)
				entities[i]->draw(ren, tmp_imgs[vector_index_2]->tex);
		}
		else if (vector_index_1 != -1)
		{
			entities[i]->draw(ren, tmp_imgs[vector_index_1]->tex);
		}
	}
}

void EntitySystem::delete_entity(int x, int y, Camera *cam, VirtualGrid *v_grid)
{
	Entity *tmp_en = get_entity(x, y, cam);
	if (!tmp_en)
		return;

	if (!tmp_en->is_deletable)
		return;

	// searching for the index in the vector of the entity @ x, y
	int index = -1;
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->id == tmp_en->id)
		{
			index = i;
			break;
		}
	}

	SDL_Point final_coo{ entities[index]->pos.x, entities[index]->pos.y };

	SDL_Point num_rows_cols = tmp_en->num_rows_cols;
	delete entities[index];
	entities.erase(entities.begin() + index);
	v_grid->set_to_empty(final_coo.x + cam->pos.x + 5, final_coo.y + cam->pos.y + 5,
		num_rows_cols.x, num_rows_cols.y);

	// refreshing all ids of entities since one of them got deleted.
	for (unsigned int i = 0; i < entities.size(); i++)
		entities[i]->id = i;
}

void EntitySystem::clear_all_entities(VirtualGrid *v_grid)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
		v_grid->set_to_empty(entities[i]->pos.x + 5, entities[i]->pos.y + 5, entities[i]->num_rows_cols.x, entities[i]->num_rows_cols.y);
	}

	entities.erase(entities.begin(), entities.end());
}

void EntitySystem::add_entity(EntityType type, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam, Utils *ut, Graph *gp)
{
	if (type == EntityType::NONE)
		return;

	Entity *tmp_en = nullptr;
	bool if_traffic_light = false;
	SDL_Point final_traffic_coo;

	bool is_vehicle = is_en_type_a_vehicle(type);
	if (is_vehicle)
	{
		tmp_en = get_entity(x, y, cam);
		if (tmp_en && tmp_en->type == EntityType::ROAD)
			tmp_en = make_entity_from_type(type);
		else
			return;
	}

	else if (type == EntityType::TRAFFIC_LIGHT)
	{
		if_traffic_light = true;
		Entity *tmp_at_en = get_entity(x, y, cam);
		if (!tmp_at_en)
			return;
		if (tmp_at_en->type == EntityType::JUNCTION ||
			tmp_at_en->type == EntityType::ROAD_4_JUNC)
		{

			SDL_Point mid_point{ tmp_at_en->pos.x + tmp_at_en->pos.w / 2,
				tmp_at_en->pos.y + tmp_at_en->pos.h / 2 };

			final_traffic_coo = { mid_point.x - grid->per_width,
				mid_point.y - grid->per_height };

			tmp_en = new TrafficLight;
			((TrafficLight*)(tmp_en))->junc_id = tmp_at_en->id;
		}
	}

	else if (type == EntityType::PAVEMENT)
	{
		SDL_Point tmp_p = grid->get_grid_pos(x, y);
		int index = v_grid->get_index(tmp_p.x + cam->pos.x + 2, tmp_p.y + cam->pos.y + 2);
		SDL_Point tmp_rows_cols = ut->type_rows_col_map[type];
		std::vector<Entity *> tmp_ens = get_entities_surrounding_index(index, tmp_rows_cols.x, tmp_rows_cols.y, v_grid);

		// check if the adjacent any of the indices belong to either road or pavement
		// only then add the pavement.

		bool is_possible = false;
		int count = 0;
		// Note: check all the four sides before placing the pavement
		// because any one of the sides may contain entity other than
		for (unsigned int i = 0; i < tmp_ens.size(); i++)
		{
			switch (tmp_ens[i]->type)
			{
			case EntityType::PAVEMENT:
			case EntityType::ROAD:
			case EntityType::BUS_STAND:
				count++;
				break;
			}
		}
		if (count == tmp_ens.size() && count != 0)
		{
			Entity *tmp_en = get_entity(x, y, cam); // making sure the pavement is not getting placed on other entities.
			if (!tmp_en)
			{
				is_possible = true;
			}
		}
		if (is_possible)
			tmp_en = new Pavement;
		else
			return;
	}

	else if (type == EntityType::BUS_STAND)
	{
		SDL_Point tmp_p = grid->get_grid_pos(x, y);
		int index = v_grid->get_index(tmp_p.x + cam->pos.x + 2, tmp_p.y + cam->pos.y + 2);
		SDL_Point tmp_rows_cols = ut->type_rows_col_map[type];
		std::vector<Entity *> tmp_ens = get_entities_surrounding_index(index, tmp_rows_cols.x, tmp_rows_cols.y, v_grid);

		// Note: check all the four sides before placing the pavement
		// because any one of the sides may contain entity other than
		bool is_possible = false;
		int count = 0;
		for (unsigned int i = 0; i < tmp_ens.size(); i++)
		{
			switch (tmp_ens[i]->type)
			{
			case EntityType::PAVEMENT:
				count++;
				break;
			}
		}
		if (count == tmp_ens.size() && count != 0)
		{
			Entity *tmp_en = get_entity(x, y, cam); // making sure it is not getting placed on other entities
			if (!tmp_en)
			{
				is_possible = true;
			}
		}
		if (is_possible)
			tmp_en = new BusStand;
		else
			return;
	}

	else if (type == EntityType::PERSON)
	{
		Entity *at_en = get_entity(x, y, cam);
		if (at_en->type == EntityType::PAVEMENT)
		{
			tmp_en = new Person;
			int tmp_img_index = rn->get_random_number(0, 19);
			((Person*)(tmp_en))->image_index = tmp_img_index;
			((Pavement*)(at_en))->is_occupied = true;
		}
	}

	else
	{
		SDL_Point tmp_p = grid->get_grid_pos(x, y);
		if (are_indices_empty(type, tmp_p.x, tmp_p.y, v_grid, cam, ut) == true)
			return;

		if (type == EntityType::ROAD)
		{
			tmp_en = new Road;
		}
		else if (type == EntityType::ROAD_4_JUNC)
		{
			tmp_en = new Road_4_Junc;
		}
		else if (type == EntityType::JUNCTION)
		{
			tmp_en = new Junction;
		}
		else if (type == EntityType::RECT_H)
		{
			tmp_en = new Rect_H;
		}
		else if (type == EntityType::RECT_V)
		{
			tmp_en = new Rect_V;
		}
	}

	if (!tmp_en)
		return;

	SDL_Point tmp_p = grid->get_grid_pos(x, y);
	SDL_Point final_coo;

	if (if_traffic_light)
		final_coo = final_traffic_coo;
	else
		final_coo = tmp_p;

	/* Note: Any function call added here must also be added to
	 * load_entities in utilities.cpp. */

	tmp_en->init((int)(entities.size()), final_coo.x, final_coo.y, grid, v_grid, cam);
	v_grid->set_filled(final_coo.x + cam->pos.x + 2, final_coo.y + cam->pos.y + 2, tmp_en->num_rows_cols.x, tmp_en->num_rows_cols.y);
	tmp_en->occupied_indices = v_grid->get_indices(x + cam->pos.x + 2, y + cam->pos.y + 2, tmp_en->num_rows_cols.x, tmp_en->num_rows_cols.y);
	init_en_occu_indices(tmp_en->occ_conn, tmp_en->occupied_indices, type, tmp_en->angle);

	if (tmp_en->type == EntityType::PERSON)
	{
		set_current_bs_to_person(tmp_en, gp, this);
	}

	entities.push_back(tmp_en);
}

void EntitySystem::load_entity(EntityType type, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity *tmp_en = make_entity_from_type(type);
	if (!tmp_en)
		return;

	SDL_Point tmp_p = grid->get_grid_pos(x, y);
	tmp_en->init((int)(entities.size()), tmp_p.x, tmp_p.y, grid, v_grid, cam);
	entities.push_back(tmp_en);
}

Entity *EntitySystem::get_en_with_id(int id)
{
	Entity *tmp_en = nullptr;
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->id == id)
		{
			tmp_en = entities[i];
			break;
		}
	}

	return tmp_en;
}

std::string Entity::get_id_in_string()
{
	std::string tmp = std::to_string(id);
	return tmp;
}

Entity *EntitySystem::get_entity(int x, int y, Camera *cam)
{
	std::vector<Entity *> tmp_ens = get_all_entities_at(x, y, cam);

	if (tmp_ens.size() == 0)
		return nullptr;

	Entity *tmp = get_min_click_entity(tmp_ens);
	return tmp;
}

std::vector<Entity*> EntitySystem::get_all_entities_at_index(int index)
{
	std::vector<Entity*> tmp_vec;
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		for (unsigned int j = 0; j < entities[i]->occupied_indices.size(); j++)
		{
			if (index == entities[i]->occupied_indices[j])
			{
				tmp_vec.push_back(entities[i]);
			}
		}
	}

	return tmp_vec;
}

Entity *EntitySystem::get_entity_at_index(int index)
{
	Entity *en = nullptr;
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		for (unsigned int j = 0; j < entities[i]->occupied_indices.size(); j++)
		{
			if (index == entities[i]->occupied_indices[j])
			{
				en = entities[i];
				break;
			}
		}
	}

	return en;
}

Entity *EntitySystem::get_next_entity(Entity *from_en, Direction dir, VirtualGrid *v_grid)
{
	SDL_Point num_rc = from_en->num_rows_cols;
	int in = -1;
	if (dir == Direction::RIGHT)
		in = from_en->occupied_indices[0] + num_rc.y; // y : num_cols
	else if (dir == Direction::LEFT)
		in = from_en->occupied_indices[0] - 1;
	else if (dir == Direction::DOWN)
		in = from_en->occupied_indices[0] + num_rc.x * v_grid->virtual_num_cols; // x : num_rows
	else if (dir == Direction::UP)
		in = from_en->occupied_indices[0] - v_grid->virtual_num_cols;

	Entity *en = get_entity_at_index(in);
	return en;
}

std::vector<Entity *> EntitySystem::get_entities_surrounding_index(int index, int x_index_len, int y_index_len, VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins = v_grid->get_adjacent_indices(index, x_index_len, y_index_len);
	// Note: 0:left, 1:right, 2:down, 3:up

	Entity *right_en = nullptr, *left_en = nullptr;
	Entity *up_en = nullptr, *down_en = nullptr;

	std::vector<Entity*> tmp_vec;
	if (tmp_ins[1] != -1)
	{
		tmp_vec = get_all_entities_at_index(tmp_ins[1]);
		for (unsigned int i = 0; i < tmp_vec.size(); i++)
		{
			bool is_vehicle = is_en_type_a_vehicle(tmp_vec[i]->type);
			if (!is_vehicle)
			{
				right_en = tmp_vec[i];
				break;
			}
		}
	}
	if (tmp_ins[0] != -1)
	{
		tmp_vec = get_all_entities_at_index(tmp_ins[0]);
		for (unsigned int i = 0; i < tmp_vec.size(); i++)
		{
			bool is_vehicle = is_en_type_a_vehicle(tmp_vec[i]->type);
			if (!is_vehicle)
			{
				left_en = tmp_vec[i];
				break;
			}
		}
	}
	if (tmp_ins[2] != -1)
	{
		tmp_vec = get_all_entities_at_index(tmp_ins[2]);
		for (unsigned int i = 0; i < tmp_vec.size(); i++)
		{
			bool is_vehicle = is_en_type_a_vehicle(tmp_vec[i]->type);
			if (!is_vehicle)
			{
				down_en = tmp_vec[i];
				break;
			}
		}
	}
	if (tmp_ins[3] != -1)
	{
		tmp_vec = get_all_entities_at_index(tmp_ins[3]);
		for (unsigned int i = 0; i < tmp_vec.size(); i++)
		{
			bool is_vehicle = is_en_type_a_vehicle(tmp_vec[i]->type);
			if (!is_vehicle)
			{
				up_en = tmp_vec[i];
				break;
			}
		}
	}

	std::vector<Entity *> ens;

	if (right_en)
		ens.push_back(right_en);
	if (left_en)
		ens.push_back(left_en);
	if (down_en)
		ens.push_back(down_en);
	if (up_en)
		ens.push_back(up_en);

	// Note: no nullptr pushed

	return ens;
}

std::vector<Entity *> EntitySystem::get_entities_surrounding_entity(Entity *en, VirtualGrid *v_grid)
{
	std::vector<Entity *> ens;
	if (!en)
		return ens;

	std::vector<int> adj_ins = en->get_required_adjacent_indices(v_grid);
	for (unsigned int i = 0; i < adj_ins.size(); i++)
	{
		std::vector<Entity*> tmp_all_ens = get_all_entities_at_index(adj_ins[i]);
		for (unsigned int j = 0; j < tmp_all_ens.size(); j++)
		{
			if (tmp_all_ens[j])
				ens.push_back(tmp_all_ens[j]);
		}
	}

	return ens;
}

std::vector<Entity *> EntitySystem::get_all_entities_at(int x, int y, Camera *cam)
{
	int r_x = x + cam->pos.x;
	int r_y = y + cam->pos.y;

	std::vector<Entity *> tmp_ens;
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		int x_1 = entities[i]->pos.x + cam->pos.x;
		int y_1 = entities[i]->pos.y + cam->pos.y;
		int x_w = x_1 + entities[i]->pos.w;
		int y_h = y_1 + entities[i]->pos.h;

		if (r_x > x_1 && r_x < x_w && r_y > y_1 && r_y < y_h)
			tmp_ens.push_back(entities[i]);
	}

	return tmp_ens;
}

Entity *EntitySystem::get_min_click_entity(std::vector<Entity *> &ens)
{
	// get entity with min click_priority
	int min = 50; // sufficiently high
	int index = 0;
	for (unsigned int i = 0; i < ens.size(); i++)
	{
		if (ens[i]->click_priority < min)
		{
			index = i;
			min = ens[i]->click_priority;
		}
	}

	return ens[index];
}

bool EntitySystem::are_indices_empty(EntityType type, int x, int y, VirtualGrid *v_grid, Camera *cam, Utils *ut)
{
	SDL_Point tmp_rows_cols = ut->type_rows_col_map[type];

	bool is_true = v_grid->is_filled(x + cam->pos.x + 5, y + cam->pos.y + 5, tmp_rows_cols.x, tmp_rows_cols.y);
	return is_true;
}

void EntitySystem::destroy()
{
	for (unsigned int i = 0; i < entities.size(); i++)
		delete entities[i];
}

void TrafficLight::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x + grid->per_width / 2, y + grid->per_height / 2, grid, v_grid, cam);
	type = EntityType::TRAFFIC_LIGHT;
	click_priority = 1;
	type_in_string = "traffic_light";

	num_rows_cols = { 2, 2 };
	current_arrow_direction = Direction::NONE;
	is_angle_mf = false;

	pos.w = grid->per_width * (num_rows_cols.y - 1);
	pos.h = grid->per_height * (num_rows_cols.x - 1);
}

std::string TrafficLight::get_delaytime_in_string()
{
	return std::to_string(time_delay);
}

int TrafficLight::get_arrow_index()
{
	if (current_arrow_direction == Direction::UP)
		return 5;
	else if (current_arrow_direction == Direction::DOWN)
		return 2;
	else if (current_arrow_direction == Direction::RIGHT)
		return 4;
	else if (current_arrow_direction == Direction::LEFT)
		return 3;

	return -1; // Direction::NONE
}

void TrafficLight::goto_next_traffic_pattern()
{
	// @future: have a customisable pattern.
	/* Pattern:
	 * Red
	 * Green - up, down, left, right
	 */

	if (!is_green)
	{
		is_green = true;
	}

	if (is_green && current_arrow_direction == Direction::NONE)
	{
		current_arrow_direction = Direction::UP;
	}
	else if (is_green && current_arrow_direction == Direction::UP)
	{
		current_arrow_direction = Direction::DOWN;
	}
	else if (is_green && current_arrow_direction == Direction::DOWN)
	{
		current_arrow_direction = Direction::LEFT;
	}
	else if (is_green && current_arrow_direction == Direction::LEFT)
	{
		current_arrow_direction = Direction::RIGHT;
	}
	else if (is_green && current_arrow_direction == Direction::RIGHT)
	{
		current_arrow_direction = Direction::NONE;
		is_green = false;
	}
}

int TrafficLight::get_tl_color_index()
{
	if (is_green)
		return 1;

	return 0; // is_red
}

std::vector<int> TrafficLight::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	return std::vector<int>();
}

void Road::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::ROAD;
	click_priority = 2;
	type_in_string = "road";

	num_rows_cols = { 2, 2 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Road::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] - 1); // top left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y); // right up
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols); // up left
	// down left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);

	return tmp_ins;
}

void Road::connect_en(Entity *to_en)
{
	if (to_en->type == EntityType::ROAD)
		connect_both_road_and_road(this, to_en);
	else if (to_en->type == EntityType::JUNCTION)
		connect_both_road_and_junction(this, to_en);
	else if (to_en->type == EntityType::ROAD_4_JUNC)
		connect_both_road_and_road4junc(this, to_en);
	else if (to_en->type == EntityType::RECT_H)
		connect_both_recth_and_road(to_en, this);
	else if (to_en->type == EntityType::RECT_V)
		connect_both_rectv_and_road(to_en, this);
}

void Road_4_Junc::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::ROAD_4_JUNC;
	click_priority = 2;
	type_in_string = "road_4_junc";

	num_rows_cols = { 4, 4 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Road_4_Junc::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] + v_grid->virtual_num_cols - 1); // left: 0
	// right: 1
	tmp_ins.push_back(occupied_indices[0] + v_grid->virtual_num_cols + num_rows_cols.y);
	// up: 2
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols + 1);
	// down: 3
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols + 1);

	return tmp_ins;
}

void Road_4_Junc::connect_en(Entity *to_en)
{
	if (to_en->type == EntityType::ROAD)
		connect_both_road_and_road4junc(this, to_en);
	else if (to_en->type == EntityType::JUNCTION)
		connect_both_junction_and_road4junc(this, to_en);
	else if (to_en->type == EntityType::ROAD_4_JUNC)
		connect_both_road4junc_and_road4junc(this, to_en);
	else if (to_en->type == EntityType::RECT_H)
		connect_both_recth_and_road4junc(to_en, this);
	else if (to_en->type == EntityType::RECT_V)
		connect_both_rectv_and_road4junc(to_en, this);
}

void Junction::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::JUNCTION;
	click_priority = 2;
	type_in_string = "junction";

	num_rows_cols = { 2, 2 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Junction::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] - 1); // left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y); // right
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols); // up
	// down
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);

	return tmp_ins;
}

void Junction::connect_en(Entity *to_en)
{
	if (to_en->type == EntityType::ROAD)
		connect_both_road_and_junction(this, to_en);
	else if (to_en->type == EntityType::JUNCTION)
		connect_both_junction_and_junction(this, to_en);
	else if (to_en->type == EntityType::ROAD_4_JUNC)
		connect_both_junction_and_road4junc(this, to_en);
	else if (to_en->type == EntityType::RECT_H)
		connect_both_recth_and_junction(to_en, this);
	else if (to_en->type == EntityType::RECT_V)
		connect_both_rectv_and_junction(to_en, this);
}
void Car::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Vehicle::init(id, x, y, grid, v_grid, cam);
	type = EntityType::CAR;
	type_in_string = "car";
	num_rows_cols = { 1, 1 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

void Bus::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Vehicle::init(id, x, y, grid, v_grid, cam);
	type = EntityType::BUS;
	type_in_string = "bus";
	num_rows_cols = { 1, 1 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

void Vehicle::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::NONE;
	click_priority = 1;
	current_move_direction = Direction::NONE;
}

std::vector<int> Vehicle::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] - 1); // left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y); // right
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols); // up
	// down
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);

	return tmp_ins;
}

bool Vehicle::move(SDL_Point p, VirtualGrid *v_grid, Camera *cam)
{
	/* Note: both p and tmp_p are mid_point of a grid_index */
	/* Note: both p and tmp_p are mid_point of a grid_index */

	SDL_Point tmp_p{ pos.x + v_grid->per_width / 2 + cam->pos.x, pos.y + v_grid->per_height / 2 + cam->pos.y };
	SDL_Point to_check_p{ p.x , p.y };

	// Car has reached point p
	if (if_within_range(tmp_p.x, to_check_p.x, 2) && if_within_range(tmp_p.y, to_check_p.y, 2))
	{
		return true;
	}

	int x_by = 0, y_by = 0;
	if (current_move_direction == Direction::RIGHT)
		x_by = v_grid->per_width / 2;
	else if (current_move_direction == Direction::LEFT)
		x_by = -v_grid->per_width / 2;
	else if (current_move_direction == Direction::DOWN)
		y_by = v_grid->per_height / 2;
	else if (current_move_direction == Direction::UP)
		y_by = -v_grid->per_height / 2;
	Entity::update_indices(current_move_direction, v_grid, tmp_p.x + x_by, tmp_p.y + y_by);

	int to_move_x = 0;
	int to_move_y = 0;

	// move left
	if (tmp_p.x > to_check_p.x && if_within_range(to_check_p.y, tmp_p.y, 2))
	{
		angle = Angle::TWO_SEVENTY;
		to_move_x = -speed;
		current_move_direction = Direction::LEFT;
	}
	// move right
	else if (tmp_p.x < to_check_p.x && if_within_range(to_check_p.y, tmp_p.y, 2))
	{
		angle = Angle::NINETY;
		to_move_x = speed;
		current_move_direction = Direction::RIGHT;
	}
	// move up
	else if (tmp_p.y > to_check_p.y && if_within_range(tmp_p.x, to_check_p.x, 2))
	{
		angle = Angle::ZERO;
		to_move_y = -speed;
		current_move_direction = Direction::UP;
	}
	// move down
	else if (tmp_p.y < to_check_p.y && if_within_range(tmp_p.x, to_check_p.x, 2))
	{
		angle = Angle::HUN_EIGHTY;
		to_move_y = speed;
		current_move_direction = Direction::DOWN;
	}

	pos.x += to_move_x;
	pos.y += to_move_y;

	return false;
}

void Pavement::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::PAVEMENT;
	click_priority = 2;
	type_in_string = "pavement";

	num_rows_cols = { 1, 1 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Pavement::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] - 1); // left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y); // right
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols); // up
	// down
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);

	return tmp_ins;
}

void Rect_H::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::RECT_H;
	click_priority = 1;
	type_in_string = "road_h";
	is_angle_mf = false;

	num_rows_cols = { 1, 2 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Rect_H::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	// up-left
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols);
	// down-right
	tmp_ins.push_back(occupied_indices[0] + v_grid->virtual_num_cols + 1);
	return tmp_ins;
}

void Rect_H::connect_en(Entity *to_en)
{
	if (to_en->type == EntityType::ROAD)
		connect_both_recth_and_road(this, to_en);
	else if (to_en->type == EntityType::JUNCTION)
		connect_both_recth_and_junction(this, to_en);
	else if (to_en->type == EntityType::ROAD_4_JUNC)
		connect_both_recth_and_road4junc(this, to_en);
}

void Rect_V::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::RECT_V;
	click_priority = 1;
	type_in_string = "road_v";
	is_angle_mf = false;

	num_rows_cols = { 2, 1 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Rect_V::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	// left-up
	tmp_ins.push_back(occupied_indices[0] - 1);
	// right-down
	tmp_ins.push_back(occupied_indices[0] + v_grid->virtual_num_cols + 1);
	return tmp_ins;
}

void Rect_V::connect_en(Entity *to_en)
{
	if (to_en->type == EntityType::ROAD)
		connect_both_rectv_and_road(this, to_en);
	else if (to_en->type == EntityType::JUNCTION)
		connect_both_rectv_and_junction(this, to_en);
	else if (to_en->type == EntityType::ROAD_4_JUNC)
		connect_both_rectv_and_road4junc(this, to_en);
}

void Person::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::PERSON;
	click_priority = 1;
	type_in_string = "person";

	num_rows_cols = { 1, 1 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> Person::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	tmp_ins.push_back(occupied_indices[0] - 1); // left
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y); // right
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols); // up
	// down
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);

	return tmp_ins;
}

void BusStand::init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam)
{
	Entity::init(id, x, y, grid, v_grid, cam);
	type = EntityType::BUS_STAND;
	click_priority = 1;
	type_in_string = "bus_stand";

	num_rows_cols = { 2, 2 };

	pos.w = grid->per_width * num_rows_cols.y;
	pos.h = grid->per_height * num_rows_cols.x;
}

std::vector<int> BusStand::get_required_adjacent_indices(VirtualGrid *v_grid)
{
	std::vector<int> tmp_ins;

	// We need all the 8 adjacent indices ..... for person and pavement and bus_stand connection stuff
	// We need all the 8 adjacent indices ..... for person and pavement and bus_stand connection stuff
	//
	// Starting from the index above occupied_indices[0] and going clockwise.

	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols);
	tmp_ins.push_back(occupied_indices[0] - v_grid->virtual_num_cols + 1);
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y);
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.y + v_grid->virtual_num_cols);
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols + 1);
	tmp_ins.push_back(occupied_indices[0] + num_rows_cols.x * v_grid->virtual_num_cols);
	tmp_ins.push_back(occupied_indices[0] - 1 + v_grid->virtual_num_cols);
	tmp_ins.push_back(occupied_indices[0] - 1);

	return tmp_ins;
}
