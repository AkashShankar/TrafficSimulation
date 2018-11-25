#include "pch.h"
#include "editor.h"

#include "Entity.h"
#include "Grid.h"
#include "utilities.h"
#include "properties.h"
#include "graph.h"
#include "query.h"
#include "camera.h"
#include "simulation.h"
#include "DB_Connection.h"

void Editor::init()
{
	display = new Display;
	display->init("Editor", 800, 600);

	text_input_rect.x = 10;
	text_input_rect.y = display->window_height - 50;
	text_input_rect.w = display->window_width - 20;
	text_input_rect.h = 40;

	cursor_rect.x = text_input_rect.x + 10;
	cursor_rect.y = text_input_rect.y + 7;
	cursor_rect.w = 10;
	cursor_rect.h = text_input_rect.h - 15;

	tex_rect.x = cursor_rect.x - 1;
	tex_rect.y = cursor_rect.y - 5;

	src_vehicle_index = des_vehicle_index = -1;
	src_vehicle_rect = { 0, 0, 0, 0 };
	des_vehicle_rect = { 0, 0, 0, 0 };

	start = clock();

	// Create Icons and add it to the icons vector
	Icon *tmp = new RoadIcon;
	tmp->init(50, 50, display->ren);
	icons.push_back(tmp);

	tmp = new Road4JuncIcon;
	tmp->init(150, 50, display->ren);
	icons.push_back(tmp);

	tmp = new JunctionIcon;
	tmp->init(250, 50, display->ren);
	icons.push_back(tmp);

	tmp = new PavementIcon;
	tmp->init(350, 50, display->ren);
	icons.push_back(tmp);

	tmp = new RectHIcon;
	tmp->init(450, 50, display->ren);
	icons.push_back(tmp);

	tmp = new RectVIcon;
	tmp->init(550, 50, display->ren);
	icons.push_back(tmp);

	tmp = new TrafficLightIcon;
	tmp->init(650, 50, display->ren);
	icons.push_back(tmp);

	tmp = new OrangeCarIcon;
	tmp->init(50, 150, display->ren);
	icons.push_back(tmp);

	tmp = new PoliceCarIcon;
	tmp->init(150, 150, display->ren);
	icons.push_back(tmp);

	tmp = new ViperCarIcon;
	tmp->init(250, 150, display->ren);
	icons.push_back(tmp);

	tmp = new AudiCarIcon;
	tmp->init(350, 150, display->ren);
	icons.push_back(tmp);

	tmp = new PersonIcon;
	tmp->init(450, 150, display->ren);
	icons.push_back(tmp);

	tmp = new BusStandIcon;
	tmp->init(550, 150, display->ren);
	icons.push_back(tmp);

	tmp = new BusIcon;
	tmp->init(650, 150, display->ren);
	icons.push_back(tmp);

	tmp = new PropertiesIcon;
	tmp->init(50, 250, display->ren);
	icons.push_back(tmp);

	tmp = new StartIcon;
	tmp->init(150, 250, display->ren);
	icons.push_back(tmp);

	tmp = new DeleteIcon;
	tmp->init(250, 250, display->ren);
	icons.push_back(tmp);

	tmp = new SaveIcon;
	tmp->init(350, 250, display->ren);
	icons.push_back(tmp);

	tmp = new LoadIcon;
	tmp->init(450, 250, display->ren);
	icons.push_back(tmp);

	tmp = new QueryIcon;
	tmp->init(550, 250, display->ren);
	icons.push_back(tmp);

	tmp = new SetPathIcon;
	tmp->init(650, 250, display->ren);
	icons.push_back(tmp);

	tmp = new SetBusIcon;
	tmp->init(50, 350, display->ren);
	icons.push_back(tmp);

	tmp = new SetPsBstIcon;
	tmp->init(150, 350, display->ren);
	icons.push_back(tmp);
}

void Editor::draw()
{
	SDL_RenderClear(display->ren);
	highlight_current_icon();

	for (unsigned int i = 0; i < icons.size(); i++)
		icons[i]->draw(display->ren);

	if (show_text_input_rect)
	{
		time_elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

		SDL_SetRenderDrawColor(display->ren, 255, 200, 0, 255);
		SDL_RenderFillRect(display->ren, &text_input_rect);

		if(time_elapsed >= 0.5f)
		{
			display_cursor = !display_cursor;
			start = clock();
		}
		if(display_cursor) 
		{
			SDL_SetRenderDrawColor(display->ren, 0, 0, 0, 255);
			SDL_RenderFillRect(display->ren, &cursor_rect);
		}

		if (input_text)
			SDL_RenderCopy(display->ren, input_text->tex, NULL, &tex_rect);
	}

	SDL_SetRenderDrawColor(display->ren, 255, 255, 255, 255);
	SDL_RenderPresent(display->ren);

	if (prop_display)
		prop_display->draw();
}

void Editor::destroy()
{
	display->destroy();
	destroy_prop();

	if (input_text)
		input_text->destroy();

	for (unsigned int i = 0; i < icons.size(); i++)
		delete icons[i];
}

void Editor::destroy_prop()
{
	if (prop_display)
		prop_display->destroy();
}

void Editor::highlight_current_icon()
{
	if (!currently_selected_icon)
		return;

	SDL_SetRenderDrawColor(display->ren, 150, 0, 0, 255);

	SDL_Rect tmp{currently_selected_icon->pos.x - 10, currently_selected_icon->pos.y - 10, currently_selected_icon->pos.w + 20, currently_selected_icon->pos.h + 20};

	SDL_RenderFillRect(display->ren, &tmp);
}

void Editor::check_and_set_current_icon(int x, int y)
{
	for (unsigned int i = 0; i < icons.size(); i++)
		if (icons[i]->was_clicked(x, y))
			currently_selected_icon = icons[i];
}

void Editor::take_action(EntitySystem *e_sys, int x, int y, Grid *grid, VirtualGrid *v_grid, Camera *cam, Utils *ut, Graph *gp, Simulation *sim)
{
	if (!currently_selected_icon)
		return;

	EntityType tmp_en_type = get_entity_type_from_icon_type(currently_selected_icon->type, ut);
	switch (currently_selected_icon->type)
	{
		case IconType::ROAD_4_JUNC:
		case IconType::ROAD:
		case IconType::JUNCTION:
		case IconType::PAVEMENT:
		case IconType::PERSON:
		case IconType::RECT_H:
		case IconType::RECT_V:
		case IconType::TRAFFIC_LIGHT:
		case IconType::BUS_STAND:
		case IconType::BUS:
			e_sys->add_entity(tmp_en_type, x, y, grid, v_grid, cam, ut, gp);
			return;
	}

	int vehicle_image_index = -1;
	switch (currently_selected_icon->type)
	{
		case IconType::ORANGE_CAR:
			vehicle_image_index = 0;
			break;
		case IconType::AUDI_CAR:
			vehicle_image_index = 1;
			break;
		case IconType::POLICE_CAR:
			vehicle_image_index = 2;
			break;
		case IconType::VIPER_CAR:
			vehicle_image_index = 3;
			break;
	}

	if(vehicle_image_index != -1)
	{
		e_sys->add_entity(tmp_en_type, x, y, grid, v_grid, cam, ut, gp);
		((Car*)(e_sys->entities[e_sys->entities.size() - 1]))->vehicle_image_index = vehicle_image_index;
		return;
	}

	else if (currently_selected_icon->type == IconType::PROPERTIES)
	{
		Entity *tmp_en = e_sys->get_entity(x, y, cam);
		if (!tmp_en)
			return;
		if (prop_display)
		{
			prop_display->destroy();
			prop_display = nullptr;
		}

		prop_display = new Properties;
		prop_display->init("Properties", 800, 400);
		prop_display->init_entity_properties(tmp_en, "Assets\\times.ttf", 25);
	}

	else if (currently_selected_icon->type == IconType::DELETE)
	{
		e_sys->delete_entity(x, y, cam, v_grid);
	}
	else if (currently_selected_icon->type == IconType::SET_PATH)
	{
		check_and_set_vehicle_indices(x, y, e_sys, cam, v_grid, grid);
	}
	else if (currently_selected_icon->type == IconType::SET_BUS)
	{
		check_and_select_bus(x, y, e_sys, cam);
	}
	else if (currently_selected_icon->type == IconType::SET_PS_BST)
	{
		check_and_set_pns_and_des_bst(x, y, e_sys, cam, sim);
	}
}

void Editor::on_editor_click_take_action(EntitySystem *e_sys, Grid *grid, VirtualGrid *v_grid, Camera *cam, Graph *gp, Simulation *sim, DB_Connection *db_con)
{
	if (!currently_selected_icon)
		return;

	if (currently_selected_icon->type == IconType::SAVE)
	{
		save_entities(e_sys, "Saves\\test.bin", cam, db_con);
	}
	else if (currently_selected_icon->type == IconType::LOAD)
	{
		load_entities(e_sys, "Saves\\test.bin", grid, v_grid, cam, gp, sim, db_con);
	}
	else if(currently_selected_icon->type == IconType::START)
	{
		start_sim = !start_sim;
	}
}

void Editor::query_icon_action(std::string &str, Query *qu)
{
	if (currently_selected_icon->type == IconType::QUERY)
	{
		show_text_input_rect = !show_text_input_rect;
		if (show_text_input_rect)
		{
			SDL_StartTextInput();
		}
		else
		{
			qu->exe_query();
			str = "";            
			SDL_StopTextInput();
			input_text->destroy();
			cursor_rect.x = text_input_rect.x + 10;
		}
	}
}

void Editor::update_input_text(std::string str)
{
	if (input_text)
		delete input_text;

	input_text = new Text;
	input_text->init(display->ren, "Assets\\times.ttf", str, 25);
	SDL_QueryTexture(input_text->tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

	cursor_rect.x = text_input_rect.x + 10 + tex_rect.w;
}

void Editor::check_and_highlight_vehicle_indices(SDL_Renderer *ren)
{
	if(src_vehicle_index != -1)
	{
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 100);
		SDL_RenderFillRect(ren, &src_vehicle_rect);
	}

	if(des_vehicle_index != -1)
	{
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 100);
		SDL_RenderFillRect(ren, &des_vehicle_rect);
	}
}

void Editor::move_vehicle_rects_with_cam(Camera *cam)
{
	if(src_vehicle_index != -1)
		cam->move_point_with_camera(src_vehicle_rect.x, src_vehicle_rect.y);
	if(des_vehicle_index != -1)
		cam->move_point_with_camera(des_vehicle_rect.x, des_vehicle_rect.y);
}

void Editor::check_and_set_vehicle_indices(int x, int y, EntitySystem *e_sys, Camera *cam, VirtualGrid *v_grid, Grid *grid)
{
	Entity *tmp_en = e_sys->get_entity(x, y, cam);
	if(!tmp_en || tmp_en->type != EntityType::CAR && !vehicle_locked)
		return;

	if(!vehicle_locked)
	{
		src_vehicle_index = tmp_en->occupied_indices[0];
		vehicle_en = tmp_en;
		((Car*)(vehicle_en))->src_index = src_vehicle_index;
		src_vehicle_rect.x = tmp_en->pos.x;
		src_vehicle_rect.y = tmp_en->pos.y;
		src_vehicle_rect.w = grid->per_width;
		src_vehicle_rect.h = grid->per_height;
		vehicle_locked = true;
		return;
	}

	// vehicle has been locked, setting des_index
	if(tmp_en->type == EntityType::ROAD && !reset_ev)
	{
		des_vehicle_index = v_grid->get_index(x + cam->pos.x + 2, y + cam->pos.y + 2);
		((Car*)(vehicle_en))->des_index = des_vehicle_index;
		SDL_Point tmp_p = grid->get_grid_pos(x, y);
		des_vehicle_rect.x = tmp_p.x;
		des_vehicle_rect.y = tmp_p.y;
		des_vehicle_rect.w = grid->per_width;
		des_vehicle_rect.h = grid->per_height;
		reset_ev = true;
		return;
	}

	if(reset_ev)
	{
		vehicle_locked = false;
		vehicle_en = nullptr;
		des_vehicle_index = src_vehicle_index = -1;
		src_vehicle_rect = {0, 0, 0, 0};
		des_vehicle_rect = {0, 0, 0, 0};
		reset_ev = false;
	}
}

void Editor::check_and_select_bus(int x, int y, EntitySystem *e_sys, Camera *cam)
{
	if(bus_locked && done_selecting_busstands)
	{
		bus_locked = false;
		bus_en = nullptr;
		bus_highlight_rect = { 0, 0, 0, 0 };
		return;
	}

	Entity *tmp_en = e_sys->get_entity(x, y, cam);
	if(tmp_en && tmp_en->type == EntityType::BUS)
	{
		bus_locked = true;
		bus_en = tmp_en;
		bus_highlight_rect = tmp_en->pos;
		done_selecting_busstands = false;
	}
}

void Editor::check_and_add_busstands(int x, int y, EntitySystem *e_sys, Camera *cam, VirtualGrid *v_grid, Grid *grid, Graph *gp)
{
	if(bus_locked && !done_selecting_busstands)
	{
		Entity *tmp_en = e_sys->get_entity(x, y, cam);

		if(tmp_en && tmp_en->type == EntityType::BUS_STAND)
		{
			((Bus*)(bus_en))->bus_stands.push_back(tmp_en->id);
		}
		else if(!tmp_en)
		{
			done_selecting_busstands = true;
		}
	}
}

void Editor::check_and_highlight_pns_and_des_bst(SDL_Renderer *ren)
{
	if(person_locked)
	{
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 100);
		SDL_RenderFillRect(ren, &pns_rect);
	}

	if(pns_bst_en)
	{
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 100);
		SDL_RenderFillRect(ren, &pns_bst_rect);
	}
}

void Editor::check_and_set_pns_and_des_bst(int x, int y, EntitySystem *e_sys, Camera *cam, Simulation *sim)
{
	static int count = 0;
	count = count + 1;

	/* Setting Person */
	{
		Entity *tmp_en = e_sys->get_entity(x, y, cam);
		if(tmp_en && tmp_en->type == EntityType::PERSON)
		{
			if(!person_locked)	
			{
				person_locked = true;
				pns_rect = tmp_en->pos;
				pns_en = tmp_en;
			}
		}
	}
	/* Setting Person */

	/* Setting des_bus_stand_id */
	{
		if(count == 2)
		{
			Entity *tmp_en = e_sys->get_entity(x, y, cam);
			if(!tmp_en || tmp_en->type != EntityType::BUS_STAND)
				return;

			if(pns_bst_en)
				return;

			pns_bst_en = tmp_en;
			pns_bst_rect = tmp_en->pos;
			((Person*)(pns_en))->des_bus_stand_id = tmp_en->id;

			// test
			int tmp_src_bst_id = ((Person*)(pns_en))->current_bus_stand_id;
			int tmp_des_bst_id = ((Person*)(pns_en))->des_bus_stand_id;
			std::vector<Entity*> all_busses = get_all_bus_that_goes_through(tmp_src_bst_id, tmp_des_bst_id, sim);

			std::cout << "bus_ids are: ";
			for(unsigned int i = 0; i < all_busses.size(); i++)
			{
				((Person*)(pns_en))->bordable_bus_ids.push_back(all_busses[i]->id);

				std::cout << all_busses[i]->id << " ";
			}
			std::cout << std::endl;
		}
	}
	/* Setting des_bus_stand_id */

	if(count > 2 && person_locked && pns_bst_en)
	{
		count = 0;

		person_locked = false;
		pns_rect = { 0, 0, 0, 0 };
		pns_bst_rect = { 0, 0, 0, 0 };
		pns_en = nullptr;
		pns_bst_en = nullptr;
	}
}

void Editor::check_and_highlight_bus(SDL_Renderer *ren) // MainDisplay
{
	if(bus_locked)
	{
		SDL_SetRenderDrawColor(ren, 100, 0, 50, 100);
		SDL_RenderFillRect(ren, &bus_highlight_rect);
	}
}

void Icon::init(int x, int y, SDL_Renderer *ren)
{
	pos.x = x;
	pos.y = y;
	pos.w = 50;
	pos.h = 50;
	image = new Image;
}

bool Icon::was_clicked(int x, int y)
{
	if (x > pos.x && x < pos.x + pos.w && y > pos.y && y < pos.y + pos.h)
		return true;

	return false;
}

void Icon::draw(SDL_Renderer *ren)
{
	SDL_Rect bor_rect { pos.x - 3, pos.y - 3, pos.w + 6, pos.h + 6 };
	SDL_SetRenderDrawColor(ren, 150, 150, 150, 100);
	SDL_RenderFillRect(ren, &bor_rect);

	SDL_RenderCopy(ren, image->tex, NULL, &pos);
}

void RoadIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::ROAD;
	image->init("Assets\\test_road_2.png", ren);
}

void Road4JuncIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::ROAD_4_JUNC;
	image->init("Assets\\road_1.png", ren);
}

void OrangeCarIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::ORANGE_CAR;
	image->init("Assets\\orange_car.png", ren);
}

void PoliceCarIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::POLICE_CAR;
	image->init("Assets\\police.png", ren);
}

void AudiCarIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::AUDI_CAR;
	image->init("Assets\\Audi.png", ren);
}

void ViperCarIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::VIPER_CAR;
	image->init("Assets\\black_viper.png", ren);
}

void JunctionIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::JUNCTION;
	image->init("Assets\\junction.jpg", ren);
}

void PropertiesIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::PROPERTIES;
	image->init("Assets\\exclamation.png", ren);
}

void DeleteIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::DELETE;
	image->init("Assets\\delete.png", ren);
}

void StartIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::START;
	image->init("Assets\\start.png", ren);
}

void PavementIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::PAVEMENT;
	image->init("Assets\\pavement.png", ren);
}

void SaveIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::SAVE;
	image->init("Assets\\save.png", ren);
}

void LoadIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::LOAD;
	image->init("Assets\\load.png", ren);
}

void PersonIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::PERSON;
	image->init("Assets\\people\\p_1.png", ren);
}

void QueryIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::QUERY;
	image->init("Assets\\query.png", ren);
}

void RectHIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	pos.h /= 2;
	type = IconType::RECT_H;
	image->init("Assets\\rect_h.png", ren);
}

void RectVIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	pos.w /= 2;
	type = IconType::RECT_V;
	image->init("Assets\\rect_v.png", ren);
}

void SetPathIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::SET_PATH;
	image->init("Assets\\set_path.png", ren);
}

void TrafficLightIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::TRAFFIC_LIGHT;
	image->init("Assets\\traffic_light.jpg", ren);
}

void BusStandIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::BUS_STAND;
	image->init("Assets\\stand.png", ren);
}

void BusIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::BUS;
	image->init("Assets\\bus.png", ren);
}

void SetBusIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::SET_BUS;
	image->init("Assets\\set_bus.png", ren);
}

void SetPsBstIcon::init(int x, int y, SDL_Renderer *ren)
{
	Icon::init(x, y, ren);
	type = IconType::SET_PS_BST;
	image->init("Assets\\pns_bst.png", ren);
}
