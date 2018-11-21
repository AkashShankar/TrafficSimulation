#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL.h>
#include <map>
#include <queue>

struct EntitySystem;
struct Entity;
struct Editor;
struct Graph;
struct VirtualGrid;
struct Camera;

struct Bus_Stops_Path
{
	std::vector<SDL_Point> path;
	std::vector<int> stop_indices;
	int current_check_index = -1;
	double record_time = 0.0;
	double stop_time = 2.0f;
};

struct Simulation
{
	// set it manually
	EntitySystem *e_sys = nullptr;
	Editor *ed = nullptr;
	Graph *gp = nullptr;
	Camera *cam = nullptr;
	VirtualGrid *v_grid = nullptr;
	double time_elapsed = 0.0;

	std::vector<Entity*> car_ens;
	std::map<Entity*, std::vector<SDL_Point>> car_paths;
	std::map<Entity*, int> car_current_path_index;

	std::vector<Entity*> bus_ens;
	std::map<Entity*, Bus_Stops_Path> bus_paths;
	std::map<Entity*, int> bus_current_path_index;
	/* Note: the bus has to wait for 2 or more seconds at each stop */

	std::vector<Entity*> bus_stands;

	std::map<Entity*, bool> en_has_crossed_junc;
	std::vector<Entity*> traffic_signal_ens;
	SDL_Thread *sim_run_th = nullptr;
};

void init(Simulation *sim);
void run(Simulation *sim);
Entity* get_en_at_index(Simulation *sim, int in);
Entity* get_traffic_signal_at_index(Simulation *sim, int in);
void update_has_crossed_junc(Simulation *sim, Entity *junc_en, Entity *tmp_en);
Entity* get_bus_stand_with_id(int id, Simulation *sim);

#endif
