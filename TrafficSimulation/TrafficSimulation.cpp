#include "pch.h"
#include "init_sdl.h"
#include "display.h"
#include "handle.h"

#include "entity.h"
#include "grid.h"
#include "image.h"
#include "editor.h"
#include "main_display.h"
#include "utilities.h"
#include "camera.h"
#include "properties.h"
#include "graph.h"
#include "simulation.h"
#include "query.h"
#include "DB_Connection.h"

struct Test : public Handle
{
	void init_update();
	void update();
	void render();
	void events();
};

Test test;
MainDisplay main_display;
Editor editor;
Grid grid;
VirtualGrid v_grid;
EntitySystem en_sys;
Camera camera;
Graph graph;
SDL_Point map_size{ 3000, 3000 };
Query query;
std::vector<Display *> all_displays;
Utils utils;
Simulation sim;
bool return_pressed = false;
DB_Connection db_con;

int simulation_run(void *ptr);

int main(int argc, char **argv)
{
	init_sdl();

	editor.init();
	main_display.init();

	en_sys.init(main_display.display->ren);

	graph.e_sys = &en_sys;
	graph.init();

	query.en_sys = &en_sys;
	query.graph = &graph;
	query.v_grid = &v_grid;

	all_displays.push_back(main_display.display);
	all_displays.push_back(editor.display);

	grid.init(main_display.display->window_height / 50,
		main_display.display->window_width / 50);
	v_grid.init(main_display.display->ren, map_size.x, map_size.y);

	main_display.entity_system = &en_sys;
	main_display.grid = &grid;
	main_display.ed = &editor;
	main_display.v_grid = &v_grid;
	main_display.gp = &graph;
	main_display.query = &query;

	camera.init(map_size.x, map_size.y);
	camera.cam_speed = grid.per_width;

	utils.init_utils();

	db_con.init();

	sim.e_sys = &en_sys;
	sim.ed = &editor;
	sim.gp = &graph;
	sim.cam = &camera;
	sim.v_grid = &v_grid;

	sim.sim_run_th = SDL_CreateThread(simulation_run, "sim_run", NULL);
	int sim_run_th_value = 0;

	SDL_StopTextInput();

	// Working.
	test.loop();

	main_display.destroy();
	editor.destroy();
	v_grid.destroy();
	db_con.destroy();
	SDL_WaitThread(sim.sim_run_th, &sim_run_th_value);
	std::cout << "Ended successfully" << std::endl;
	quit_sdl();
	return 0;
}

void Test::init_update()
{
	camera.current_dir_state = Direction::NONE;
	return_pressed = false;
}

void Test::update()
{
	if (camera.current_dir_state != Direction::NONE)
	{
		camera.move_camera();

		for (unsigned int i = 0; i < en_sys.entities.size(); i++)
		{
			camera.move_point_with_camera(en_sys.entities[i]->pos.x,
				en_sys.entities[i]->pos.y);
		}

		v_grid.move_text_pos(camera.current_dir_state);
		graph.move_car_src_des_index_with_cam(&camera, &grid, &v_grid);
		editor.move_vehicle_rects_with_cam(&camera);
	}
}

void Test::render()
{
	main_display.draw();
	editor.draw();
}

void Test::events()
{
	for (unsigned int i = 0; i < all_displays.size(); i++)
	{
		if (ev.type == SDL_WINDOWEVENT && ev.window.windowID == all_displays[i]->win_id)
		{
			switch (ev.window.event)
			{
			case SDL_WINDOWEVENT_ENTER:
				all_displays[i]->shown = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				all_displays[i]->shown = false;
				break;
			}
		}
	}

	if (ev.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (editor.display->shown)
		{
			editor.check_and_set_current_icon(x, y);
			editor.query_icon_action(query.query_str, &query);
		}
		else if (main_display.display->shown)
		{
			if (ev.button.button == SDL_BUTTON_RIGHT)
			{
				graph.set_car_src_des_indices(&editor, &v_grid, &camera, x, y);
			}
			if (ev.button.button == SDL_BUTTON_LEFT)
			{
				editor.take_action(&en_sys, x, y, &grid, &v_grid, &camera, &utils, &graph, &sim);
				editor.check_and_add_busstands(x, y, &en_sys, &camera, &v_grid, &grid, &graph);
			}
		}
		else if (editor.prop_display)
		{
			editor.prop_display->check_and_change_angle(x, y);
			editor.prop_display->check_and_change_is_del_and_arrow_pos(x, y);
			editor.prop_display->check_and_change_delay_time_arrow_pos(x, y);
		}
	}

	if (ev.type == SDL_TEXTINPUT && editor.show_text_input_rect)
	{
		query.query_str += ev.text.text;
		editor.update_input_text(query.query_str);
	}

	else if (ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_BACKSPACE && query.query_str.length() > 0 && editor.show_text_input_rect)
		{
			query.query_str = query.query_str.substr(0, query.query_str.length() - 1);
			editor.update_input_text(query.query_str);
		}

		else if (!editor.show_text_input_rect)
		{
			if (ev.key.keysym.sym == SDLK_1)
			{
				main_display.display->show();
			}
			else if (ev.key.keysym.sym == SDLK_2)
			{
				editor.display->show();
			}
			else if (ev.key.keysym.sym == SDLK_3)
			{
				editor.destroy_prop();
			}
			else if (ev.key.keysym.sym == SDLK_x)
			{
				v_grid.show_filled_indices();
			}
			else if (ev.key.keysym.sym == SDLK_RETURN)
			{
				editor.on_editor_click_take_action(&en_sys, &grid, &v_grid,
					&camera, &graph, &sim, &db_con);
				return_pressed = true;
			}
			else if (ev.key.keysym.sym == SDLK_w && camera.pos.y > 0)
			{
				camera.current_dir_state = Direction::UP;
			}
			else if (ev.key.keysym.sym == SDLK_s &&
				camera.pos.y + grid.grid_height <= camera.limit.y)
			{
				camera.current_dir_state = Direction::DOWN;
			}
			else if (ev.key.keysym.sym == SDLK_a && camera.pos.x > 0)
			{
				camera.current_dir_state = Direction::LEFT;
			}
			else if (ev.key.keysym.sym == SDLK_d &&
				camera.pos.x + grid.grid_width <= camera.limit.x)
			{
				camera.current_dir_state = Direction::RIGHT;
			}
		}
	}
}

int simulation_run(void *ptr)
{
	while (test.running)
	{
		if (editor.start_sim && !return_pressed && editor.currently_selected_icon->type != IconType::LOAD)
		{
			SDL_Delay(10);
			run(&sim);
		}
	}

	return 0;
}
