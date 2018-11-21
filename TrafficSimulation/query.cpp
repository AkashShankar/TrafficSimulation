#include "pch.h"
#include <SDL.h>
#include "query.h"
#include "display.h"
#include "entity.h"
#include "utilities.h"
#include "graph.h"
#include "grid.h"

#include "boost\\algorithm\\string\\split.hpp"
#include "boost\\algorithm\\string\\classification.hpp"

Query::Query()
{
	q_type_string[QueryType::NONE] = "none";
	q_type_string[QueryType::CLEAR_ALL] = "clear_all";
	q_type_string[QueryType::HIGHLIGHT_ALL_TYPES] = "highlight_all_entities_with_types";
	q_type_string[QueryType::HIGHLIGHT_WITH_ID] = "highlight_entity_with_id";
	q_type_string[QueryType::SHOW_CONNECTIONS_WITH_ID] = "show_connections_with_id";
	q_type_string[QueryType::SHOW_INTERNAL_CONNECTIONS_OF_ID] = "show_internal_connections_of_id";
	q_type_string[QueryType::SHOW_FILLED_INDICES] = "show filled_indices";
}

void Query::exe_query()
{
	words.erase(words.begin(), words.end());
	current_query_type = QueryType::NONE;

	if (query_str == "")
		return;

	figure_out_query_type();

	std::cout << "\n -----------------------------\n" << std::endl;

	if (current_query_type == QueryType::HIGHLIGHT_WITH_ID)
	{
		int en_id = std::stoi(words[words.size() - 1]);
		// checking the entity_system if there is any entity with id en_id
		for (unsigned int i = 0; i < en_sys->entities.size(); i++)
		{
			if (en_id == en_sys->entities[i]->id)
			{
				en_index = i;
				break;
			}
		}
	}

	else if (current_query_type == QueryType::HIGHLIGHT_ALL_TYPES)
	{
		en_type = get_type_from_str(words[words.size() - 1]);
		all_type_indices.erase(all_type_indices.begin(), all_type_indices.end());

		for (unsigned int i = 0; i < en_sys->entities.size(); i++)
		{
			if (en_sys->entities[i]->type == en_type)
				all_type_indices.push_back(i);
		}
	}

	else if(current_query_type == QueryType::SHOW_CONNECTIONS_WITH_ID)
	{
		std::cout << "id: " << words[words.size() - 1] << std::endl;
		std::cout << "connections are: ";
		int tmp_id = std::stoi(words[words.size() - 1]);
		Node *tmp_node = graph->get_node_with_id(tmp_id);
		if(tmp_node)
		{
			for(unsigned int i = 0; i < tmp_node->adjacent_nodes.size(); i++)
			{
				std::cout << tmp_node->adjacent_nodes[i]->id << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "--------" << std::endl;
	}

	else if(current_query_type == QueryType::SHOW_INTERNAL_CONNECTIONS_OF_ID)
	{
		std::cout << "id: " << words[words.size() - 1] << std::endl;
		std::cout << "interal connections are: " << std::endl;
		int tmp_id = std::stoi(words[words.size() - 1]);
		Node *tmp_node = graph->get_node_with_id(tmp_id);
		show_all_occ_en(tmp_node->en);
	}

	else if(current_query_type == QueryType::SHOW_FILLED_INDICES)
	{
		std::cout << "Filled_indices are: ";
		for(unsigned int i = 0; i < v_grid->filled_indices.size(); i++)
		{
			if(v_grid->filled_indices[i])
				std::cout << i << " ";
		}
		std::cout << std::endl;
	}

	print_current_query_type();

	std::cout << "Executed query" << std::endl;
	std::cout << "-----------------------------\n" << std::endl;
}

void Query::figure_out_query_type()
{
	// split
	boost::split(words, query_str, boost::is_any_of(" "), boost::token_compress_on);

	if (query_str == "" || words.size() == 0)
		current_query_type = QueryType::NONE;
	else if (if_query_is_highlight_with_id())
		current_query_type = QueryType::HIGHLIGHT_WITH_ID;
	else if (if_query_is_hightlight_all_types())
		current_query_type = QueryType::HIGHLIGHT_ALL_TYPES;
	else if (if_query_is_show_connections_of_id())
		current_query_type = QueryType::SHOW_CONNECTIONS_WITH_ID;
	else if (if_query_is_show_internal_connections_of_id())
		current_query_type = QueryType::SHOW_INTERNAL_CONNECTIONS_OF_ID;
	else if(if_query_is_show_filled_indices())
		current_query_type = QueryType::SHOW_FILLED_INDICES;
}

bool Query::if_query_is_highlight_with_id()
{
	// Example: highlight entity with id <id>
	if (words.size() == 5)
	{
		if (words[0] != "highlight" || words[1] != "entity" || words[2] != "with" || words[3] != "id")
		{
			return false;
		}

		// check if last word is a number
		bool is_number = if_string_is_digit(words[4]);

		if (!is_number)
			return false;
	}
	else
		return false;

	return true;
}

bool Query::if_query_is_show_internal_connections_of_id()
{
	// Example: show internal connections of id <id>
	if(words.size() == 6)
	{
		if(words[0] != "show" || words[1] != "internal" || words[2] != "connections"
				|| words[3] != "of" || words[4] != "id")
		{
			return false;
		}
		// check if last word is a number
		bool is_number = if_string_is_digit(words[5]);

		if (!is_number)
			return false;
	}
	else
		return false;

	return true;
}

bool Query::if_query_is_hightlight_all_types()
{
	// Ex: highlight all entities with type <EntityType>
	if (words.size() == 6)
	{
		if (words[0] != "highlight" || words[1] != "all" || words[2] != "entities" || words[3] != "with" || words[4] != "type")
		{
			return false;
		}

		en_type = get_type_from_str(words[5]);
		if (en_type == EntityType::NONE)
			return false;
	}
	else
		return false;

	return true;
}

bool Query::if_query_is_show_connections_of_id()
{
	// Ex: show all connections of id <id>
	if (words.size() == 6)
	{
		if (words[0] != "show" || words[1] != "all" || words[2] != "connections" || words[3] != "of" || words[4] != "id")
		{
			return false;
		}

		bool is_num = if_string_is_digit(words[5]);
		if (!is_num)
			return false;
	}
	else
		return false;

	return true;
}

bool Query::if_query_is_show_filled_indices()
{
	// Ex: show filled_indices
	if(words.size() == 2)
	{
		if(words[0] != "show" || words[1] != "filled_indices")
			return false;
	}

	return true;
}

void Query::highlight_accordingly(SDL_Renderer *ren)
{
	if (current_query_type == QueryType::HIGHLIGHT_WITH_ID)
	{
		if (en_index == -1)
			return;

		SDL_SetRenderDrawColor(ren, 255, 0, 0, 150);
		SDL_RenderFillRect(ren, &en_sys->entities[en_index]->pos);
	}

	else if (current_query_type == QueryType::HIGHLIGHT_ALL_TYPES)
	{
		for (unsigned int i = 0; i < all_type_indices.size(); i++)
		{
			SDL_SetRenderDrawColor(ren, 255, 255, 0, 150);
			SDL_RenderFillRect(ren, &en_sys->entities[all_type_indices[i]]->pos);
		}
	}
}

void Query::print_current_query_type()
{
	std::string str = q_type_string[current_query_type];
	std::cout << "QueryType: " << str << std::endl;
}
