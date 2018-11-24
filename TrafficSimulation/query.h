#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "entity.h" // for EntityType


struct Display;
struct EntitySystem;
struct Renderer;
struct Graph;
struct VirtualGrid;

enum class QueryType
{
	HIGHLIGHT_WITH_ID,
	HIGHLIGHT_ALL_TYPES,
	CLEAR_ALL,
	SHOW_CONNECTIONS_WITH_ID,
	SHOW_INTERNAL_CONNECTIONS_OF_ID,
	SHOW_FILLED_INDICES,
	NONE
};

struct Query
{
	std::string query_str; // set manually
	EntitySystem *en_sys = nullptr; // set manually
	Graph *graph = nullptr;
	VirtualGrid *v_grid = nullptr;

	Query();
	void exe_query();
	void figure_out_query_type();

	/* for highlighting entity with id x */
	int en_index = -1;
	/* for highlighting entity with id x */

	/* for highlighting all entity_types */
	EntityType en_type = EntityType::NONE;
	std::vector<int> all_type_indices;
	/* for highlighting all entity_types */

	std::map<QueryType, std::string> q_type_string;

	void highlight_accordingly(SDL_Renderer *ren);

private:
	QueryType current_query_type = QueryType::NONE;
	std::vector<std::string> words;

	void print_current_query_type();
	bool if_query_is_highlight_with_id();
	bool if_query_is_hightlight_all_types();
	bool if_query_is_show_connections_of_id();
	bool if_query_is_show_internal_connections_of_id();
	bool if_query_is_show_filled_indices();
};

#endif
