#ifndef EDITOR_H
#define EDITOR_H

#include "display.h"
#include "image.h"
#include <vector>
#include <ctime>

struct EntitySystem;
struct Entity;
struct Grid;
struct VirtualGrid;
struct Camera;
struct Graph;
struct Properties;
struct Text;
struct Query;
struct Utils;
struct Simulation;
struct DB_Connection;

enum class IconType
{
	NONE, ROAD, ROAD_4_JUNC, ORANGE_CAR, POLICE_CAR, AUDI_CAR, VIPER_CAR, JUNCTION, 
	PAVEMENT, PROPERTIES, START, DELETE, SAVE, LOAD, PERSON, 
	QUERY, RECT_H, RECT_V, SET_PATH, TRAFFIC_LIGHT, BUS_STAND, BUS, SET_BUS, SET_PS_BST
};

struct Icon
{
	Image *image = nullptr;
	IconType type = IconType::NONE;
	SDL_Rect pos{0, 0, 0, 0};

	virtual void init(int x, int y, SDL_Renderer *ren);
	void draw(SDL_Renderer *ren);
	bool was_clicked(int x, int y);
};

struct RoadIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct Road4JuncIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct OrangeCarIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct PoliceCarIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct ViperCarIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct AudiCarIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct JunctionIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct PropertiesIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct StartIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct DeleteIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct PavementIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct SaveIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct LoadIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct PersonIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct QueryIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct RectHIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct RectVIcon: public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct SetPathIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct TrafficLightIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct BusStandIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct BusIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct SetBusIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct SetPsBstIcon : public Icon
{
	void init(int x, int y, SDL_Renderer *ren);
};

struct Editor
{
	Display *display = nullptr;
	std::vector<Icon*> icons;
	Icon *currently_selected_icon = nullptr;
	Properties *prop_display = nullptr;

	/* For text input */
	SDL_Rect text_input_rect { 0, 0, 0, 0, };
	SDL_Rect cursor_rect { 0, 0, 0, 0 };
	bool show_text_input_rect = false;
	Text *input_text = nullptr;
	SDL_Rect tex_rect { 0, 0, 0, 0 };
	double time_elapsed = 0.0;
	clock_t start;
	bool display_cursor = false;
	/* For text input */

	/* set_path */
	bool vehicle_locked = false;
	int des_vehicle_index = -1;
	int src_vehicle_index = -1;
	SDL_Rect src_vehicle_rect, des_vehicle_rect;
	Entity *vehicle_en = nullptr;
	bool reset_ev = false;
	/* set_path */

	Entity *first_en_node = nullptr;
	Entity *second_en_node = nullptr;

	/* Start Sim */
	bool start_sim = false;
	/* Start Sim */

	/* Set BusStands */
	bool bus_locked = false;
	Entity *bus_en = nullptr;
	SDL_Rect bus_highlight_rect;
	bool done_selecting_busstands = false;
	/* Set BusStands */

	/* Set Person's des_bus_stand */
	bool person_locked = false;
	Entity *pns_bst_en = nullptr;
	Entity *pns_en = nullptr;
	SDL_Rect pns_rect;
	SDL_Rect pns_bst_rect;
	/* Set Person's des_bus_stand */

	void init();
	void draw();
	void destroy();
	void destroy_prop();

	void highlight_current_icon(); 
	void check_and_set_current_icon(int x, int y);
	void take_action(EntitySystem *e_sys, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam, Utils *ut, Graph *gp, Simulation *sim);
	void on_editor_click_take_action(EntitySystem *e_sys, Grid *grid, VirtualGrid *v_grid, Camera *cam, Graph *gp, Simulation *sim, DB_Connection *db_con);
	void query_icon_action(std::string &str, Query *qu);
	void update_input_text(std::string str);

	/*
	void check_and_select_two_entities(EntitySystem *e_sys, Grid *grid, Camera *cam, int x, int y);
	void check_and_highlight_two_entities(SDL_Renderer *ren); // MainDisplay's ren
	*/
	void check_and_highlight_vehicle_indices(SDL_Renderer *ren); // MainDisplay's ren
	void check_and_set_vehicle_indices(int x, int y, EntitySystem *e_sys, Camera *cam, VirtualGrid *v_grid, Grid *grid);
	void move_vehicle_rects_with_cam(Camera *cam);

	void check_and_select_bus(int x, int y, EntitySystem *e_sys, Camera *cam);
	void check_and_highlight_bus(SDL_Renderer *ren); // MainDisplay's ren
	void check_and_add_busstands(int x, int y, EntitySystem *e_sys, Camera *cam, VirtualGrid *v_grid, Grid *grid, Graph *gp);

	void check_and_set_pns_and_des_bst(int x, int y, EntitySystem *e_sys, Camera *cam, Simulation *sim);
	void check_and_highlight_pns_and_des_bst(SDL_Renderer *ren); // MainDisplay's ren
};

#endif
