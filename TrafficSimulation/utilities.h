#ifndef UTILITIES_H
#define UTILITIES_H

#include <random>
#include <map>

#include "editor.h"
#include "entity.h"
#include "simulation.h"

struct Graph;
struct Camera;
struct Simulation;

enum class Direction
{
	LEFT, RIGHT, DOWN, UP, NONE
};

struct RandomNumbers
{
	private:
		std::random_device rd;
		std::mt19937 *gen = nullptr;
	public:
		void init();
		std::vector<int> get_uniform_dis(int from, int to, int times);
		int get_random_number(int from, int to);
		void print_uniform_dis(int from, int to, int times);
		~RandomNumbers();
		RandomNumbers();
};

struct Utils
{
	std::map<EntityType, IconType> en_ic_map;
	std::map<IconType, EntityType> ic_en_map;
	std::map<EntityType, SDL_Point> type_rows_col_map; // point x:rows, y:cols

	void init_utils();
};

IconType get_icon_type_from_entity_type(EntityType type, Utils *ut);
EntityType get_entity_type_from_icon_type(IconType type, Utils *ut);
EntityType get_type_from_str(std::string str);
std::string get_str_from_type(EntityType type);
std::string get_str_from_angle(Angle angle);
Angle get_angle_from_str(std::string str);
Entity* make_entity_from_type(EntityType type);
bool if_string_is_digit(std::string str);
bool is_en_type_a_vehicle(EntityType type);

int get_occ_index_attached_to_pavement(Entity *pave_en, Graph *gp, VirtualGrid *v_grid);
void update_current_stand_for_bus(Entity *bus_en, const std::vector<Entity*> &bus_stands, Graph *gp, VirtualGrid *v_grid);

Entity* get_pavement_next_to_road_with_bst_id(int id, Graph *gp);
Entity* get_entity_next_to_type_with_id(int id, EntityType type, Graph *gp);
Bus_Stops_Path generate_bus_path(Entity *e, EntitySystem *e_sys, Graph *gp, VirtualGrid *v_grid, Camera *cam);
Image* make_image(std::string str, SDL_Renderer *ren);
inline bool check_and_assign_types(Entity *e1, Entity *e2, EntityType t1, EntityType t2, Entity* &a1, Entity* &a2);

std::vector<Entity*> get_all_bus_that_goes_through(int src_bst_id, int des_bst_id, Simulation *sim);

std::vector<Entity*> get_pavements_next_to_pavement(Entity *pave_en, Graph *gp);
void get_all_pavements_rc(std::vector<Entity*> &vec, Entity *start_en, Graph *gp, std::vector<int> &vtd_ids);
// std::vector<Entity*> get_pavements_next_to_busstand(int bs_id, Graph *gp, EntitySystem *e_sys);
void set_current_bs_to_person(Entity *ps_en, Graph *gp, EntitySystem *e_sys);

bool is_coo_within_rect(int x, int y, const SDL_Rect &rect);
void save_entities(EntitySystem *en_sys, std::string f_name, Camera *cam);
void load_entities(EntitySystem *en_sys, std::string f_name, Grid *grid, VirtualGrid *v_grid, Camera *cam, Graph *gp, Simulation *sim);
bool if_connected(EntitySystem *en_sys, VirtualGrid *v_grid, Entity *e1, Entity *e2);
bool if_angle_is_left_or_right(Angle a1, Angle a2);
bool if_angle_is_up_or_down(Angle a1, Angle a2);

bool if_road_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_road_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_road4_junc_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_junc_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_junc_connected_to_road4_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_junc_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);

bool if_busstand_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);

bool if_recth_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_recth_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_recth_connected_to_road4junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);

bool if_rectv_connected_to_road(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_rectv_connected_to_junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_rectv_connected_to_road4junc(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);

bool if_road_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);
bool if_pavement_connected_to_pavement(EntitySystem *e_sys, Entity *e1, Entity *e2, VirtualGrid *v_grid);

Direction get_direction_of_entities(Entity *source_en, Entity *des_en);
bool if_within_range(int i1, int i2, int dev);

bool if_e1_surrounding_e2(EntitySystem *e_sys, VirtualGrid *v_grid, Entity *e1, Entity *e2);

std::map<int , std::vector<int>> get_en_internal_connections(EntityType type, Angle angle);
void init_en_occu_indices(std::map<std::map<int, int>, std::vector<int>> &oc, const std::vector<int> &occupied_indices, EntityType type, Angle angle);

void connect_both_road_and_road(Entity *e1, Entity *e2);
void connect_both_road_and_junction(Entity *e1, Entity *e2);
void connect_both_road_and_road4junc(Entity *e1, Entity *e2);
void connect_both_junction_and_road4junc(Entity *e1, Entity *e2);
void connect_both_junction_and_junction(Entity *e1, Entity *e2);
void connect_both_road4junc_and_road4junc(Entity *e1, Entity *e2);
void connect_both_recth_and_road(Entity *e1, Entity *e2);
void connect_both_recth_and_junction(Entity *e1, Entity *e2);
void connect_both_recth_and_road4junc(Entity *e1, Entity *e2);
void connect_both_rectv_and_road(Entity *e1, Entity *e2);
void connect_both_rectv_and_junction(Entity *e1, Entity *e2);
void connect_both_rectv_and_road4junc(Entity *e1, Entity *e2);

void add_index_to_occ_conn(Entity *from_en, Entity *to_en, int from_in, int to_in);

// tmp
void print_angle(Angle ang);
void print_entity_type(EntityType type);
void print_rect(SDL_Rect *rect);
void print_direction(Direction dir);
void show_all_occ_en(Entity *en);

#endif
