#include "pch.h"
#include "properties.h"

#include "entity.h"
#include "text.h"
#include "image.h"
#include "utilities.h"

void Properties::init(std::string title, int w, int h)
{
    display = new Display;
    display->init(title, w, h);
    display->shown = false;

    right_angle_image = new Image;
    left_angle_image = new Image;

    right_angle_image->init("Assets\\arrowRight.png", display->ren);
    left_angle_image->init("Assets\\arrowLeft.png", display->ren);

    border_rect.x = 10;
    border_rect.y = 10;
    border_rect.w = w - 20;
    border_rect.h = h - 20;
}

void Properties::draw()
{
    SDL_RenderClear(display->ren);

    SDL_SetRenderDrawColor(display->ren, 80, 80, 150, 255);
    SDL_RenderFillRect(display->ren, &border_rect);

    show_window();

    SDL_SetRenderDrawColor(display->ren, 100, 100, 100, 255);
    SDL_RenderPresent(display->ren);
}

void Properties::init_entity_properties(Entity *en, std::string font_name, int size)
{
    this->entity = en;
    this->font_name = font_name;
    this->size = size;

    if(!en)
    {
        std::cout << "Entity is a nullptr init_entity_properties()" << std::endl;
        return;
    }

    // type
    type_text = new Text;
    type_text->col = { 200, 200, 0, 255 };
    std::string type_string_text = "Type: ";
    type_string_text += en->type_in_string;
    type_text->init(display->ren, font_name, type_string_text, size);
    SDL_QueryTexture(type_text->tex, NULL, NULL, &type_pos.w, &type_pos.h);
    type_pos.x = display->window_width / 2 - type_pos.w / 2; type_pos.y = 40;

    // pos_text
    pos_text = new Text;
    pos_text->col = { 200, 200, 0, 255 };
    std::string pos_string_text = "position: ";
    pos_string_text += en->get_pos_in_string();
    pos_text->init(display->ren, font_name, pos_string_text, size);
    SDL_QueryTexture(pos_text->tex, NULL, NULL, &pos_pos.w, &pos_pos.h);
    pos_pos.x = display->window_width / 2 - pos_pos.w / 2; pos_pos.y = 80;

    // angle
    update_angle_text_and_arrow_pos();

    // right_image and left_angle next to angle.
    right_angle_pos.w = 30;
    right_angle_pos.h = 30;
    left_angle_pos.w = 30;
    left_angle_pos.h = 30;
    right_angle_pos.x = angle_pos.x + angle_pos.w + 50;
    right_angle_pos.y = angle_pos.y;
    left_angle_pos.x = angle_pos.x + angle_pos.w + 10;
    left_angle_pos.y = angle_pos.y;

    // indices
    indices_text = new Text;
    indices_text->col = {200, 200, 0, 255};
    std::string indices_string_text = "indices: ";
    indices_string_text += en->get_indices_in_string();
    indices_text->init(display->ren, font_name, indices_string_text, size);
    SDL_QueryTexture(indices_text->tex, NULL, NULL, &indices_pos.w, &indices_pos.h);
    indices_pos.x = display->window_width / 2 - indices_pos.w / 2;
    indices_pos.y = 160;

    // id
    id_text = new Text;
    id_text->col = {200, 200, 0, 255};
    std::string id_string_text = "id: ";
    id_string_text += en->get_id_in_string();
    id_text->init(display->ren, font_name, id_string_text, size);
    SDL_QueryTexture(id_text->tex, NULL, NULL, &id_pos.w, &id_pos.h);
    id_pos.x = display->window_width / 2 - id_pos.w / 2;
    id_pos.y = 200;

    // is_deletable and is_deletable_arrow_pos
    update_is_del_text();

	 // delay_time for TrafficLight
	 delay_time_left_arrow_pos.w = delay_time_left_arrow_pos.h = 30;
	 delay_time_right_arrow_pos.w = delay_time_right_arrow_pos.h = 30;
	 update_delaytime_and_arrow_pos();
}

void Properties::check_and_change_angle(int x, int y)
{
    if (!entity)
        return;

	 if(entity->is_angle_mf == false)
		 return;

    if (is_coo_within_rect(x, y, right_angle_pos))
    {
        entity->goto_next_angle();
        update_angle_text_and_arrow_pos();
    }

    else if (is_coo_within_rect(x, y, left_angle_pos))
    {
        entity->goto_previous_angle();
        update_angle_text_and_arrow_pos();
    }
}

void Properties::check_and_change_delay_time_arrow_pos(int x, int y)
{
	if(entity && entity->type == EntityType::TRAFFIC_LIGHT)
	{
		int tmp_delay_time = ((TrafficLight*)(entity))->time_delay;
		if (is_coo_within_rect(x, y, delay_time_left_arrow_pos))
		{
			((TrafficLight*)(entity))->time_delay = tmp_delay_time - 1;
		}
		else if (is_coo_within_rect(x, y, delay_time_right_arrow_pos))
		{
			((TrafficLight*)(entity))->time_delay = tmp_delay_time + 1;
		}
		update_delaytime_and_arrow_pos();
	}
}

void Properties::check_and_change_is_del_and_arrow_pos(int x, int y)
{
	if (!entity)
		return;

	if (is_coo_within_rect(x, y, is_deletable_arrow_pos))
	{
		entity->is_deletable = !entity->is_deletable;
		update_is_del_text();
	}
}

void Properties::update_angle_text_and_arrow_pos()
{
	if (angle_text)
		delete angle_text;
	angle_text = nullptr;

	angle_text = new Text;
	angle_text->col = {200, 200, 0, 255};
	std::string angle_string_text = "angle: ";
	angle_string_text += entity->get_angle_in_string();
	angle_text->init(display->ren, font_name, angle_string_text, size);
	SDL_QueryTexture(angle_text->tex, NULL, NULL, &angle_pos.w, &angle_pos.h);
	angle_pos.x = display->window_width / 2 - angle_pos.w / 2;
	angle_pos.y = 120;

	right_angle_pos.x = angle_pos.x + angle_pos.w + 50;
	right_angle_pos.y = angle_pos.y;
	left_angle_pos.x = angle_pos.x + angle_pos.w + 10;
	left_angle_pos.y = angle_pos.y;
}

void Properties::update_delaytime_and_arrow_pos()
{
	if(delay_time_text)
		delete delay_time_text;
	delay_time_text = nullptr;

	delay_time_text = new Text;
	delay_time_text->col = {200, 200, 0, 255};
	std::string delay_time_string_text = "delay_time: ";
	delay_time_string_text += entity->get_delaytime_in_string();
	delay_time_text->init(display->ren, font_name, delay_time_string_text, size);
	SDL_QueryTexture(delay_time_text->tex, NULL, NULL, &delay_time_pos.w, 
			&delay_time_pos.h);
	delay_time_pos.x = display->window_width / 2 - delay_time_pos.w / 2;
	delay_time_pos.y = 280;

	delay_time_left_arrow_pos.x = delay_time_pos.x + delay_time_pos.w + 10;
	delay_time_left_arrow_pos.y = delay_time_pos.y;
	delay_time_right_arrow_pos.x = delay_time_pos.x + delay_time_pos.w + 50;
	delay_time_right_arrow_pos.y = delay_time_pos.y;
}

void Properties::update_is_del_text()
{
	if (is_deletable_text)
		delete is_deletable_text;
	is_deletable_text = nullptr;

	is_deletable_text = new Text;
	is_deletable_text->col = {200, 200, 0, 255};
	std::string is_del_string = "is_deletable: ";
	is_del_string += entity->get_del_in_string();
	is_deletable_text->init(display->ren, font_name, is_del_string, size);
	SDL_QueryTexture(is_deletable_text->tex, NULL, NULL, &is_deletable_pos.w, &is_deletable_pos.h);
	is_deletable_pos.x = display->window_width / 2 - is_deletable_pos.w / 2;
	is_deletable_pos.y = 240;

	is_deletable_arrow_pos.w = 30;
	is_deletable_arrow_pos.h = 30;
	is_deletable_arrow_pos.x = is_deletable_pos.x + is_deletable_pos.w + 10;
	is_deletable_arrow_pos.y = is_deletable_pos.y;
}

void Properties::show_window()
{
	if (!entity)
		return;

	SDL_RenderCopy(display->ren, type_text->tex, NULL, &type_pos);
	SDL_RenderCopy(display->ren, pos_text->tex, NULL, &pos_pos);
	SDL_RenderCopy(display->ren, angle_text->tex, NULL, &angle_pos);
	SDL_RenderCopy(display->ren, indices_text->tex, NULL, &indices_pos);
	SDL_RenderCopy(display->ren, id_text->tex, NULL, &id_pos);
	SDL_RenderCopy(display->ren, is_deletable_text->tex, NULL, &is_deletable_pos);

	SDL_RenderCopy(display->ren, right_angle_image->tex, NULL, &right_angle_pos);
	SDL_RenderCopy(display->ren, right_angle_image->tex, NULL, &is_deletable_arrow_pos);
	SDL_RenderCopy(display->ren, left_angle_image->tex, NULL, &left_angle_pos);

	if(entity->type == EntityType::TRAFFIC_LIGHT)
	{
		SDL_RenderCopy(display->ren, left_angle_image->tex, NULL, &delay_time_left_arrow_pos);
		SDL_RenderCopy(display->ren, right_angle_image->tex, NULL, &delay_time_right_arrow_pos);
		SDL_RenderCopy(display->ren, delay_time_text->tex, NULL, &delay_time_pos);
	}
}

void Properties::destroy()
{
	if (display)
		display->destroy();
	if (type_text)
		type_text->destroy();
	if (pos_text)
		pos_text->destroy();
	if (angle_text)
		angle_text->destroy();
	if (indices_text)
		indices_text->destroy();
	if (is_deletable_text)
		is_deletable_text->destroy();

	right_angle_image->destroy();
	left_angle_image->destroy();
}
