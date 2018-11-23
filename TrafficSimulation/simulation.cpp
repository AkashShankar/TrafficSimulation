#include "pch.h"
#include "simulation.h"
#include "entity.h"
#include "editor.h"
#include "graph.h"
#include "grid.h"

#include <assert.h>
#include <vector>

void init(Simulation *sim)
{
	/* For every movable_entity get_its src and des index, get both the roads
	 * at src and des index. Then set the car_src_index, car_des_index to current
	 * vehicle entitie's respective src, des index. Set src_clicked and des_clicked
	 * for graph to true ( otherwise compute_shortest_path will return ).
	 *
	 * the function compute_shortest_path has to return a std::queue<SDL_Point>
	 * that has the mid_point of every path_grid_index from src to des
	 */ 

	/* For now: the path will just be computed once at the start of the simulation
	 * Later we'll update the path at regular intervals of time
	 */

	/* Call this function after the map is loaded ( load_entities() in utilities.cpp ) */

	assert(sim->e_sys != nullptr);
	assert(sim->ed != nullptr);
	assert(sim->gp != nullptr);
	assert(sim->cam != nullptr);
	assert(sim->v_grid != nullptr);

	sim->gp->add_all_nodes(sim->e_sys, sim->v_grid);
	sim->gp->compute_adjacency_matrix();

	for(unsigned int i = 0; i < sim->e_sys->entities.size(); i++)
	{
		EntityType current_type = sim->e_sys->entities[i]->type;
		Entity *tmp_en = sim->e_sys->entities[i];

		if(current_type == EntityType::CAR)
			sim->car_ens.push_back(tmp_en);
		else if(current_type == EntityType::TRAFFIC_LIGHT)
			sim->traffic_signal_ens.push_back(tmp_en);
		else if(current_type == EntityType::BUS)
			sim->bus_ens.push_back(tmp_en);
		else if(current_type == EntityType::BUS_STAND)
			sim->bus_stands.push_back(tmp_en);
		else if(current_type == EntityType::PERSON)
			sim->people.push_back(tmp_en);
	}

	sim->gp->src_clicked = true;
	sim->gp->des_clicked = true;
	for(unsigned int i = 0; i < sim->car_ens.size(); i++)
	{
		int tmp_src_index = ((Car*)(sim->car_ens[i]))->src_index;
		int tmp_des_index = ((Car*)(sim->car_ens[i]))->des_index;

		if(tmp_src_index != -1 && tmp_des_index != -1)
		{
			sim->gp->car_src_index = tmp_src_index;
			sim->gp->car_des_index = tmp_des_index;	

			std::vector<SDL_Point> tmp_vec_path = sim->gp->compute_shortest_path_bw(sim->v_grid, sim->cam);

			if(tmp_vec_path.size() >= 2) // The vehicle is initially at this position
				tmp_vec_path.erase(tmp_vec_path.begin());

			sim->car_paths[sim->car_ens[i]] = tmp_vec_path;
			sim->car_current_path_index[sim->car_ens[i]] = 0;
			sim->en_has_crossed_junc[sim->car_ens[i]] = false;
		}
	}

	/*
	// tmp: displaying entity_id and its path
	std::map<Entity*, std::vector<SDL_Point>>::iterator it = sim->car_paths.begin();
	for(; it != sim->car_paths.end(); ++it)
	{
	Entity *tmp_en = it->first;
	std::vector<SDL_Point> tmp_vec = it->second;

	std::cout << "id: " << tmp_en->id << std::endl;
	std::cout << "path: ";

	for(unsigned int i = 0; i < tmp_vec.size(); i++)
	{
	SDL_Point tmp_point = tmp_vec[i];
	std::cout << "(" << tmp_point.x << "," << tmp_point.y << "), ";
	}
	std::cout << std::endl;
	}
	*/

	// generating bus_paths
	for(unsigned int i = 0; i < sim->bus_ens.size(); i++)
	{
		Bus_Stops_Path tmp_bst = generate_bus_path(sim->bus_ens[i], sim->e_sys, sim->gp, sim->v_grid, sim->cam);

		sim->bus_paths[sim->bus_ens[i]] = tmp_bst;
		sim->bus_current_path_index[sim->bus_ens[i]] = 0;
		sim->en_has_crossed_junc[sim->bus_ens[i]] = false;
	}

	sim->gp->src_clicked = false;
	sim->gp->des_clicked = false;
}

void run(Simulation *sim)
{
	sim->time_elapsed = (double)(SDL_GetTicks()) / 1000.0f;
	// std::cout << "time_elapsed: " << sim->time_elapsed << std::endl;

	for(unsigned int i = 0; i < sim->traffic_signal_ens.size(); i++)
	{
		Entity *tmp_en = sim->traffic_signal_ens[i];
		int tmp_delay_time = ((TrafficLight*)(tmp_en))->time_delay;
		int tmp_record_time = ((TrafficLight*)(tmp_en))->record_time;

		if((int)sim->time_elapsed % tmp_delay_time == 0 && 
				(int)sim->time_elapsed != tmp_record_time)
		{
			tmp_en->goto_next_traffic_pattern();
			((TrafficLight*)(tmp_en))->record_time = ((int)sim->time_elapsed);
		}
	}

	// moving cars
	std::map<Entity*, std::vector<SDL_Point>>::iterator it = sim->car_paths.begin();
	for(; it != sim->car_paths.end(); ++it)
	{
		Entity *tmp_en = it->first;
		std::vector<SDL_Point> tmp_vec = it->second;
		unsigned int current_index = sim->car_current_path_index[tmp_en];
		SDL_Point to_point = tmp_vec[current_index];
		bool has_reached = false;

		if(current_index < tmp_vec.size())
		{
			has_reached = tmp_en->move(to_point, sim->v_grid, sim->cam);

			// checking if next index is empty
			if(current_index < tmp_vec.size() - 1)
			{
				SDL_Point p_to_check = tmp_vec[current_index + 1];

				int in_to_check = sim->v_grid->get_index(p_to_check.x, p_to_check.y);
				Entity* tmp_en_2 = get_en_at_index(sim, in_to_check);
				Entity *traffic_en = get_traffic_signal_at_index(sim, in_to_check);
			Entity *junc_en = nullptr;

				// Check if next junction has traffic_light in it
				// if yes, check if the car->move_direction is
				// same as signal->arrow_direction and proceed
				// Once the vehicle has entered the junction
				// the signal should'nt affect its movement

				if(tmp_en_2)
				{
					has_reached = false;
				}
				if(traffic_en && !sim->en_has_crossed_junc[tmp_en])
				{
					Direction d1 = ((TrafficLight*)(traffic_en))->current_arrow_direction;
					Direction d2 = ((Car*)(tmp_en))->current_move_direction;

					if(d1 == d2)
						sim->en_has_crossed_junc[tmp_en] = true;
					else
						has_reached = false;
				}

				if(traffic_en)
				{
					int tmp_junc_id = ((TrafficLight*)(traffic_en))->junc_id;
					junc_en = sim->e_sys->get_en_with_id(tmp_junc_id);
					update_has_crossed_junc(sim, junc_en, tmp_en);
				}
			}
		}

		if(has_reached && current_index < tmp_vec.size())
		{
			sim->car_current_path_index[tmp_en] = ++current_index;
		}
	}

	// moving busses
	// moving busses
	// moving busses
	// moving busses
	std::map<Entity*, Bus_Stops_Path>::iterator bus_it = sim->bus_paths.begin();
	for(; bus_it != sim->bus_paths.end(); ++bus_it)
	{
		Entity *tmp_en = bus_it->first;
		Bus_Stops_Path tmp_bst = bus_it->second;
		std::vector<SDL_Point> tmp_vec = tmp_bst.path;
		std::vector<int> stop_indices = tmp_bst.stop_indices;
		unsigned int current_index = sim->bus_current_path_index[tmp_en];
		SDL_Point to_point = tmp_vec[current_index];
		bool has_reached = false;

		double diff = abs(sim->time_elapsed - tmp_bst.record_time);
		if(diff < tmp_bst.stop_time) {
			continue;
		}

		// searching if current_index is next to any bus_stop
		bool is_in = false;
		for(unsigned int x = 0; x < stop_indices.size(); x++)
		{
			if(tmp_en->occupied_indices[0] == stop_indices[x] && tmp_bst.current_check_index != stop_indices[x])
			{
				tmp_bst.current_check_index = stop_indices[x];
				is_in = true;
			}
		}

		if(is_in) 
			tmp_bst.record_time = sim->time_elapsed;
		else 
			tmp_bst.record_time = 0;

		sim->bus_paths[tmp_en] = tmp_bst;

		if(current_index < tmp_vec.size())
		{
			has_reached = tmp_en->move(to_point, sim->v_grid, sim->cam);

			// checking if next index is empty
			if(current_index < tmp_vec.size() - 1)
			{
				SDL_Point p_to_check = tmp_vec[current_index + 1];

				int in_to_check = sim->v_grid->get_index(p_to_check.x, p_to_check.y);
				Entity* tmp_en_2 = get_en_at_index(sim, in_to_check);
				Entity *traffic_en = get_traffic_signal_at_index(sim, in_to_check);
				Entity *junc_en = nullptr;

				if(tmp_en_2)
				{
					has_reached = false;
				}
				if(traffic_en && !sim->en_has_crossed_junc[tmp_en])
				{
					Direction d1 = ((TrafficLight*)(traffic_en))->current_arrow_direction;
					Direction d2 = ((Bus*)(tmp_en))->current_move_direction;

					if(d1 == d2)
						sim->en_has_crossed_junc[tmp_en] = true;
					else
						has_reached = false;
				}

				if(traffic_en)
				{
					int tmp_junc_id = ((TrafficLight*)(traffic_en))->junc_id;
					junc_en = sim->e_sys->get_en_with_id(tmp_junc_id);
					update_has_crossed_junc(sim, junc_en, tmp_en);
				}
			}
		}

		if(has_reached && current_index < tmp_vec.size())
			sim->bus_current_path_index[tmp_en] = ++current_index;
		else if(current_index == tmp_vec.size() - 1)
			sim->bus_current_path_index[tmp_en] = 0;

		update_current_stand_for_bus(tmp_en, sim->bus_stands, sim->gp, sim->v_grid);

		/*
		if (((Bus*)(tmp_en))->current_bus_stand) {
			std::cout << "bus_stand_id for bus : " << tmp_en->id << " is: " << ((Bus*)(tmp_en))->current_bus_stand->id << std::endl;
		}
		*/
	}

	/*	Transporting people 
		For every person check all the busses, if any bus'es current_bus_stand->id matches
		with the person's current_bus_stand->id then:
		Check if the bus'es->id matched any of the person's->bordable_bus_id, If Yes then:
			* Set person->is_travelling = true;
			* Set person->current_bus_stand->id = -1;
			* Set person->current_bus_id = current_bus'es->id;
		To Check if person has reached destination:
			if person->is_travelling:
				Get the current_bus entity from person's->current_bus_id;
				check if current_bus_en->current_bus_stand == person's->des_bus_stand_id, if Yes then:
					* TMP: PUT SOME MSG TO THE CONSOLE
					* Set person->is_travelling = false;
					* Set person->current_bus_stand->id to current_bus_en->current_bus_stand->id;
					* Set person->current_bus_id = -1;
	*/

	for (unsigned int i = 0; i < sim->people.size(); i++)
	{
		Person *current_person = ((Person*)(sim->people[i]));
		if (!current_person->is_travelling && !current_person->has_reached_des)
		{
			bool is_eligible = false;
			for (unsigned int j = 0; j < sim->bus_ens.size(); j++)
			{
				Bus *current_bus = ((Bus*)(sim->bus_ens[j]));
				if (current_bus->current_bus_stand
					&& current_bus->current_bus_stand->id == current_person->current_bus_stand_id)
				{
					std::vector<int> bordable_busses_ids = current_person->bordable_bus_ids;
					for (unsigned int x = 0; x < bordable_busses_ids.size(); x++)
					{
						if (current_bus->id == bordable_busses_ids[x])
						{
							current_person->is_travelling = true;
							current_person->current_bus_stand_id = -1;
							current_person->current_bus_id = current_bus->id;
							is_eligible = true;
						}
					}
				}
				if (is_eligible)
					break;
			}
		}
		else // Person is travelling
		{
			Bus *current_bus_en = (Bus*)(get_bus_with_id(current_person->current_bus_id, sim));
			if (current_bus_en && current_bus_en->current_bus_stand &&
				current_bus_en->current_bus_stand->id == current_person->des_bus_stand_id)
			{
				std::cout << "Person with id: " << current_person->id << " has reached destination." << std::endl;
				current_person->is_travelling = false;
				current_person->current_bus_stand_id = current_bus_en->current_bus_stand->id;
				current_person->current_bus_id = -1;
				current_person->has_reached_des = true;
				current_person->des_bus_stand_id = -1; // So that they don't keep travelling
			}
		}
	}
}

void update_has_crossed_junc(Simulation *sim, Entity *junc_en, Entity* tmp_en)
{
	if (junc_en)
	{
		bool should_set_to_false = true;
		for (unsigned int i = 0; i < junc_en->occupied_indices.size(); i++)
		{
			if (tmp_en->occupied_indices[0] == junc_en->occupied_indices[i])
			{
				should_set_to_false = false;
			}
		}
		if (should_set_to_false)
		{
			sim->en_has_crossed_junc[tmp_en] = false;
		}
	}
}

Entity* get_en_at_index(Simulation *sim, int in)
{
	// car_ens
	for (unsigned int i = 0; i < sim->car_ens.size(); i++)
	{
		std::vector<int> occ_indices = sim->car_ens[i]->occupied_indices;
		for (unsigned int j = 0; j < occ_indices.size(); j++)
		{
			if (occ_indices[j] == in)
			{
				return sim->car_ens[i];
			}
		}
	}

	// also check for bus_ens
	for (unsigned int i = 0; i < sim->bus_ens.size(); i++)
	{
		std::vector<int> occ_indices = sim->bus_ens[i]->occupied_indices;
		for (unsigned int j = 0; j < occ_indices.size(); j++)
		{
			if (occ_indices[j] == in)
			{
				return sim->bus_ens[i];
			}
		}
	}

	return nullptr;
}

Entity* get_traffic_signal_at_index(Simulation *sim, int in)
{
	Entity *tmp_en = nullptr;

	for (unsigned int i = 0; i < sim->traffic_signal_ens.size(); i++)
	{
		Entity *to_check_en = sim->traffic_signal_ens[i];
		Entity *at_en = sim->e_sys->get_entity_at_index(in);

		if (at_en && to_check_en->id == at_en->id)
		{
			tmp_en = to_check_en;
			break;
		}
	}

	return tmp_en;
}

Entity* get_bus_stand_with_id(int id, Simulation *sim)
{
	Entity* tmp_bus_stand = nullptr;
	std::vector<Entity*> bus_stands = sim->bus_stands;

	for (unsigned int i = 0; i < bus_stands.size(); i++)
	{
		if (bus_stands[i]->id == id)
		{
			tmp_bus_stand = bus_stands[i];
			break;
		}
	}

	return tmp_bus_stand;
}

Entity* get_bus_with_id(int id, Simulation *sim)
{
	Entity *tmp_bus = nullptr;
	std::vector<Entity*> busses = sim->bus_ens;

	for (unsigned int i = 0; i < busses.size(); i++)
	{
		if (busses[i]->id == id)
		{
			tmp_bus = busses[i];
			break;
		}
	}

	return tmp_bus;
}
