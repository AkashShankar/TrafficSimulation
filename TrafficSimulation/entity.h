#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <vector>
#include <map>

#include "image.h"

struct Grid;
struct VirtualGrid;
struct Camera;
struct Graph;
struct RandomNumbers;
struct Utils;
enum class Direction;

enum class EntityType
{
	NONE, ROAD, ROAD_4_JUNC, CAR, JUNCTION, PAVEMENT, PERSON, RECT_H, RECT_V,
	TRAFFIC_LIGHT, BUS_STAND, BUS
};

enum class Angle
{
	ZERO, NINETY, HUN_EIGHTY, TWO_SEVENTY
};

struct Entity
{
	EntityType type = EntityType::NONE;
	std::string type_in_string = "none";
	SDL_Rect pos = { 0, 0, 0, 0 };
	SDL_Point num_rows_cols { 0, 0 };
	int id = -1;
	Angle angle = Angle::ZERO;
	std::vector<int> occupied_indices;
	int click_priority = -1;
	std::map<std::map<int, int>, std::vector<int>> occ_conn;

	bool is_angle_mf = true;
	bool is_deletable = true;

	virtual void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	virtual void draw(SDL_Renderer *ren, SDL_Texture *tex);
	virtual std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);

	virtual void connect_en(Entity *to_en);
	virtual bool move(SDL_Point p, VirtualGrid *v_grid, Camera *cam);
	virtual void goto_next_traffic_pattern();       // traffic_light
	virtual int get_arrow_index();                  // traffic_light
	virtual int get_tl_color_index();               // traffic_light
	virtual std::string get_delaytime_in_string();  // traffic_light

	std::map<int, int> get_mapped_occ_en();
	void goto_next_angle();
	void goto_previous_angle();
	void update_indices(Direction dir, VirtualGrid *v_grid, int new_x_pos, int new_y_pos);
	void set_current_indices_to_empty(VirtualGrid *v_grid);
	std::string get_pos_in_string();
	std::string get_angle_in_string();
	std::string get_indices_in_string();
	std::string get_id_in_string();
	std::string get_del_in_string();
};

struct EntitySystem
{
	std::vector<Entity*> entities;
	RandomNumbers *rn = nullptr; // for person_image_indices

	std::map<EntityType, std::vector<Image*>> en_images;

	void init(SDL_Renderer *ren);
	void draw(SDL_Renderer *ren);
	void add_entity(EntityType type, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam, Utils *ut, Graph *gp);
	void load_entity(EntityType type, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	bool are_indices_empty(EntityType type, int x, int y, VirtualGrid *v_grid, Camera *cam, Utils *ut);
	void delete_entity(int x, int y, Camera *cam, VirtualGrid *v_grid);
	void clear_all_entities(VirtualGrid *v_grid);
	Entity* get_entity(int x, int y, Camera *cam);
	Entity* get_entity_at_index(int index);
	std::vector<Entity*> get_all_entities_at_index(int index);
	Entity* get_next_entity(Entity *from_en, Direction dir, VirtualGrid *v_grid);
	Entity *get_en_with_id(int id);
	std::vector<Entity*> get_entities_surrounding_index(int index, int x_index_len, int y_index_len, VirtualGrid *v_grid);
	std::vector<Entity*> get_entities_surrounding_entity(Entity *en, VirtualGrid *v_grid);
	void destroy();

	private:
	std::vector<Entity*> get_all_entities_at(int x, int y, Camera *cam);
	Entity* get_min_click_entity(std::vector<Entity*> &ens);
};

struct Vehicle: public Entity
{
	/*
		int miles_driven = 0;
		int fuel_consumed = 0;
		int num_seats = 0;
	*/
	
	int vehicle_image_index = -1;
	int speed = 1;
	Direction current_move_direction;
	int src_index = -1;
	int des_index = -1;

	virtual void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	virtual std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	bool move(SDL_Point p, VirtualGrid *v_grid, Camera *cam);
};

struct TrafficLight: public Entity
{
	Direction current_arrow_direction;
	bool is_green = false;
	int time_delay = 3;
	int record_time = 0;
	int junc_id = -1;

	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	int get_arrow_index();
	int get_tl_color_index();
	void goto_next_traffic_pattern();
	std::string get_delaytime_in_string();
};

struct Road : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	Direction get_index_direction(int index);
	void connect_en(Entity *to_en);
};

struct Road_4_Junc : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	void connect_en(Entity *to_en);
};

struct Junction : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	void connect_en(Entity *to_en);
};

struct Pavement : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
};

struct Rect_H : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	void connect_en(Entity *to_en);
};

struct Rect_V : public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
	void connect_en(Entity *to_en);
};

struct Person: public Entity
{
	int image_index = -1;
	int current_bus_stand_id = -1;
	int des_bus_stand_id = -1;
	std::vector<int> bordable_bus_ids;

	/* bool is_male = false;
	 */

	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
};

struct BusStand: public Entity
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> get_required_adjacent_indices(VirtualGrid *v_grid);
};

struct Car: public Vehicle
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
};

struct Bus: public Vehicle
{
	void init(int id, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam);
	std::vector<int> bus_stands; // in go_order
	Entity *current_bus_stand = nullptr;
};

#endif
