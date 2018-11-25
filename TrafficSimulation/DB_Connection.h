#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include "entity.h"

#include <iostream>
#include <string>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/resultset.h>

struct DB_Reg_En
{
	int id = -1;
	EntityType type;
	Angle angle;
	int occ_index;
};

struct DB_Car
{
	DB_Reg_En reg_en;
	int car_image_index = -1;
	int speed = 1;
	float fuel_consumed = 0;
	float miles_driven = 0;
	int src_index = -1;
	int des_index = -1;
};

struct DB_Traffic_Light
{
	DB_Reg_En reg_en;
	int time_delay = 3;
	int junc_id = -1;
	int pos_x = -1;
	int pos_y = -1;
};

struct DB_Person
{
	DB_Reg_En reg_en;
	int image_index = -1;
	int current_bst_id = -1;
	int des_bst_id = -1;
	float money_spent = 0;
	std::vector<int> bus_ids;
};

struct DB_Bus
{
	DB_Reg_En reg_en;
	int speed = 1;
	float fuel_consumed = 0;
	float miles_driven = 0;
	std::vector<int> bus_stand_ids;
};

struct DB_Connection
{
	sql::Driver *driver = nullptr;
	sql::Connection *con = nullptr;
	sql::Statement *stmt = nullptr;
	sql::Statement *stmt_2 = nullptr;
	sql::ResultSet *res = nullptr;
	sql::ResultSet *res_2 = nullptr;

	std::string current_table;
	std::string current_stmt;

	std::vector<DB_Reg_En> get_all_reg_en();
	std::vector<DB_Car> get_all_car();
	std::vector<DB_Bus> get_all_bus();
	std::vector<DB_Person> get_all_person();
	std::vector<DB_Traffic_Light> get_all_traffic_light();
	std::vector<DB_Reg_En> get_all_bus_stand();

	void create_new_reg_en(int id, EntityType type, Angle angle, int occ_index);
	void create_new_car_en(int id, EntityType type, Angle angle, int occ_index, int speed, 
		float fuel_consumed, float miles_driven, int src_index, int des_index, int car_image_index);
	void create_new_traffic_light_en(int id, EntityType type, Angle angle, int occ_index,
		int time_delay, int junc_id, int pos_x, int pos_y);
	void create_new_person_en(int id, EntityType type, Angle angle, int occ_index,
		int image_index, int current_bst_id, int des_bst_id, float money_spent, std::vector<int> bus_ids);
	void create_new_bus_en(int id, EntityType type, Angle angle, int occ_index,
		int speed, float fuel_consumed, float miles_driven, std::vector<int> bus_stand_ids);
	void create_new_bus_stand_en(int id, EntityType type, Angle angle, int occ_index);

	std::string get_basic_str(std::string init_str, int id, EntityType type, Angle angle, int occ_index);
	std::string get_in_quotes(int value);

	void truncate_all();
	inline void execute_stmt(std::string str);

	void init();
	void destroy();
};

#endif
