#include "pch.h"
#include "DB_Connection.h"

#include <jdbc/cppconn/statement.h>
#include "utilities.h"

void DB_Connection::init()
{
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "Akash", "$STrongakA$");
	con->setSchema("traffic");
}

std::vector<DB_Reg_En> DB_Connection::get_all_reg_en()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from Reg_En");

	std::vector<DB_Reg_En> tmp_reg_ens;

	while (res->next())
	{
		DB_Reg_En tmp_reg_en;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);

		tmp_reg_ens.push_back(tmp_reg_en);
	}

	delete res;
	delete stmt;

	return tmp_reg_ens;
}

std::vector<DB_Car> DB_Connection::get_all_car()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from Car");
	std::vector<DB_Car> tmp_cars;

	while (res->next())
	{
		DB_Reg_En tmp_reg_en;
		DB_Car tmp_car;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);
		tmp_car.reg_en = tmp_reg_en;

		tmp_car.fuel_consumed = res->getDouble("fuel_consumed");
		tmp_car.speed = res->getInt("speed");
		tmp_car.miles_driven = res->getDouble("miles_driven");
		tmp_car.src_index = res->getInt("src_index");
		tmp_car.des_index = res->getInt("des_index");

		tmp_cars.push_back(tmp_car);
	}

	delete res;
	delete stmt;

	return tmp_cars;
}

std::vector<DB_Bus> DB_Connection::get_all_bus()
{
	stmt = con->createStatement();
	stmt_2 = con->createStatement();
	res = stmt->executeQuery("select * from Bus");
	std::vector<DB_Bus> tmp_busses;

	while (res->next())
	{
		DB_Reg_En tmp_reg_en;
		DB_Bus tmp_bus;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);
		tmp_bus.reg_en = tmp_reg_en;

		tmp_bus.speed = res->getInt("speed");
		tmp_bus.fuel_consumed = res->getDouble("fuel_consumed");
		tmp_bus.miles_driven = res->getDouble("miles_driven");

		// the bus_stands
		std::string tmp_string = "select bus_stand_id from bus_bus_stands where bus_id = ";
		tmp_string += tmp_reg_en.id;
		tmp_string += ";";

		res_2 = stmt_2->executeQuery(tmp_string);
		while (res_2->next())
		{
			tmp_bus.bus_stand_ids.push_back(res_2->getInt("bus_stand_id"));
		}

		delete res_2;

		tmp_bus.reg_en = tmp_reg_en;
	}

	delete stmt;
	delete stmt_2;
	delete res;

	return tmp_busses;
}

std::vector<DB_Person> DB_Connection::get_all_person()
{
	stmt = con->createStatement();
	stmt_2 = con->createStatement();
	res = stmt->executeQuery("select * from Person");
	std::vector<DB_Person> tmp_people;

	while (res->next())
	{
		DB_Reg_En tmp_reg_en;
		DB_Person tmp_person;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);
		tmp_person.reg_en = tmp_reg_en;

		tmp_person.image_index = res->getInt("image_index");
		tmp_person.current_bst_id = res->getInt("current_bst_id");
		tmp_person.des_bst_id = res->getInt("des_bst_id");
		tmp_person.money_spent = res->getDouble("money_spent");

		// bus_ids
		std::string tmp_string = "select bus_stand_id from person_bus_stands where person_id = ";
		tmp_string += tmp_reg_en.id;
		tmp_string += ";";

		res_2 = stmt_2->executeQuery(tmp_string);
		while (res_2->next())
		{
			tmp_person.bus_ids.push_back(res_2->getInt("bus_stand_id"));
		}

		delete res_2;

		tmp_people.push_back(tmp_person);
	}

	delete stmt;
	delete stmt_2;
	delete res;

	return tmp_people;
}

std::vector<DB_Traffic_Light> DB_Connection::get_all_traffic_light()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from TrafficLight");
	std::vector<DB_Traffic_Light> tmp_lights;
	
	while (res->next())
	{
		DB_Reg_En tmp_reg_en;
		DB_Traffic_Light tmp_light;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);
		tmp_light.reg_en = tmp_reg_en;

		tmp_light.junc_id = res->getInt("junc_id");
		tmp_light.pos_x = res->getInt("pos_x");
		tmp_light.pos_y = res->getInt("pos_y");
		tmp_light.time_delay = res->getInt("time_delay");

		tmp_lights.push_back(tmp_light);
	}

	delete stmt;
	delete res;

	return tmp_lights;
}

std::string DB_Connection::get_basic_str(std::string init_str, int id, EntityType type, 
	Angle angle, int occ_index)
{
	std::string str = init_str;
	str += std::to_string(id);
	str += ", ";
	str += get_str_from_type(type);
	str += ", ";
	str += get_str_from_angle(angle);
	str += ", ";
	str += std::to_string(occ_index);

	return str;
}

void DB_Connection::create_new_reg_en(int id, EntityType type, Angle angle, int occ_index)
{
	std::string str = get_basic_str("insert into reg_en values(", id, type, angle, occ_index);
	str += ");";

	stmt = con->createStatement();
	stmt->execute(str);
	delete stmt;
}

void DB_Connection::create_new_car_en(int id, EntityType type, Angle angle, int occ_index, int speed,
	float fuel_consumed, float miles_driven, int src_index, int des_index)
{
	std::string str = get_basic_str("insert into car values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(speed);
	str += ", ";
	str += std::to_string(fuel_consumed);
	str += ", ";
	str += std::to_string(miles_driven);
	str += ", ";
	str += std::to_string(src_index);
	str += ", ";
	str += std::to_string(des_index);

	str += ");";

	stmt = con->createStatement();
	stmt->execute(str);
	delete stmt;
}

void DB_Connection::create_new_traffic_light_en(int id, EntityType type, Angle angle, int occ_index,
	int time_delay, int junc_id, int pos_x, int pos_y)
{
	std::string str = get_basic_str("insert into traffic_light values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(time_delay);
	str += ", ";
	str += std::to_string(junc_id);
	str += ", ";
	str += std::to_string(pos_x);
	str += ", ";
	str += std::to_string(pos_y);

	str += ");";

	stmt = con->createStatement();
	stmt->execute(str);
	delete stmt;
}

void DB_Connection::create_new_person_en(int id, EntityType type, Angle angle, int occ_index,
	int image_index, int current_bst_id, int des_bst_id, float money_spent, std::vector<int> bus_ids)
{
	std::string str = get_basic_str("insert into person_light values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(image_index);
	str += ", ";
	str += std::to_string(current_bst_id);
	str += ", ";
	str += std::to_string(des_bst_id);
	str += ", ";
	str += std::to_string(money_spent);
	str += ");";

	stmt = con->createStatement();
	stmt->execute(str);
	delete stmt;

	// adding bus_ids to person_bus_stands
	for (unsigned int i = 0; i < bus_ids.size(); i++)
	{
		str = "insert into person_bus_stands values(";
		str += std::to_string(id);
		str += ", ";
		str += std::to_string(bus_ids[i]);
		str += ");";

		stmt = con->createStatement();
		stmt->execute(str);
		delete stmt;
	}
}

void DB_Connection::create_new_bus_en(int id, EntityType type, Angle angle, int occ_index,
	int speed, float fuel_consumed, float miles_driven, std::vector<int> bus_stand_ids)
{
	std::string str = get_basic_str("insert into bus values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(speed);
	str += ", ";
	str += std::to_string(fuel_consumed);
	str += ", ";
	str += std::to_string(miles_driven);
	str += ");";

	stmt = con->createStatement();
	stmt->execute(str);
	delete stmt;

	// adding bus_stand_ids to bus_bus_stands
	for (unsigned int i = 0; i < bus_stand_ids.size(); i++)
	{
		str = "insert into bus_bus_stands values(";
		str += std::to_string(id);
		str += ", ";
		str += std::to_string(bus_stand_ids[i]);
		str += ");";

		stmt = con->createStatement();
		stmt->execute(str);
		delete stmt;
	}
}

void DB_Connection::destroy()
{
	delete con;
}
