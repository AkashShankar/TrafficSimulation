#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "display.h"

struct Entity;
struct Text;
struct Image;

struct Properties
{
	Display *display = nullptr;
	Entity *entity = nullptr;
	std::string font_name;
	int size = 0;

	// Text for:
	// Type, pos, id, angle and occupied indices

	Text *type_text = nullptr;
	Text *pos_text = nullptr;
	Text *angle_text = nullptr;
	Text *indices_text = nullptr;
	Text *id_text = nullptr;
	Text *is_deletable_text = nullptr;
	Text *delay_time_text = nullptr;

	Image *right_angle_image = nullptr;
	Image *left_angle_image = nullptr;

	SDL_Rect type_pos{ 0, 0, 0, 0 };
	SDL_Rect pos_pos{ 0, 0, 0, 0 };
	SDL_Rect angle_pos{ 0, 0, 0, 0 };
	SDL_Rect indices_pos{ 0, 0, 0, 0 };
	SDL_Rect id_pos{ 0, 0, 0, 0 };
	SDL_Rect is_deletable_pos{ 0, 0, 0, 0 };
	SDL_Rect delay_time_pos{ 0, 0, 0, 0 };

	SDL_Rect border_rect{ 0, 0, 0, 0 };
	SDL_Rect right_angle_pos{ 0, 0, 0, 0 };
	SDL_Rect left_angle_pos{ 0, 0, 0, 0 };
	SDL_Rect is_deletable_arrow_pos{ 0, 0, 0, 0 };
	SDL_Rect delay_time_left_arrow_pos{ 0, 0, 0, 0 };
	SDL_Rect delay_time_right_arrow_pos{ 0, 0, 0, 0 };

	void init(std::string title, int w, int h);
	void init_entity_properties(Entity *en, std::string font_name, int size);
	void check_and_change_angle(int x, int y);
	void check_and_change_is_del_and_arrow_pos(int x, int y);
	void check_and_change_delay_time_arrow_pos(int x, int y);
	void update_angle_text_and_arrow_pos();
	void update_delaytime_and_arrow_pos();
	void update_is_del_text();
	void draw();
	void destroy();

private:
	void show_window();
};

#endif
