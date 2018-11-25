#include "pch.h"
#include "DB_Connection.h"

#include <jdbc/cppconn/statement.h>
#include <fstream>
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

std::vector<DB_Reg_En> DB_Connection::get_all_bus_stand()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("select * from BusStand");

	std::vector<DB_Reg_En> tmp_bst_ens;

	while (res->next())
	{
		DB_Reg_En tmp_reg_en;

		std::string type_in_str = res->getString("type");
		std::string angle_in_str = res->getString("angle");

		tmp_reg_en.id = res->getInt("id");
		tmp_reg_en.occ_index = res->getInt("occ_index");
		tmp_reg_en.type = get_type_from_str(type_in_str);
		tmp_reg_en.angle = get_angle_from_str(angle_in_str);

		tmp_bst_ens.push_back(tmp_reg_en);
	}

	delete res;
	delete stmt;

	return tmp_bst_ens;
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
		tmp_car.car_image_index = res->getInt("car_image_index");

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
		tmp_string += std::to_string(tmp_reg_en.id);
		tmp_string += ";";

		res_2 = stmt_2->executeQuery(tmp_string);
		while (res_2->next())
		{
			tmp_bus.bus_stand_ids.push_back(res_2->getInt("bus_stand_id"));
		}
		delete res_2;

		tmp_bus.reg_en = tmp_reg_en;
		tmp_busses.push_back(tmp_bus);
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
		std::string tmp_string = "select bus_id from person_bus where person_id = ";
		tmp_string += std::to_string(tmp_reg_en.id);
		tmp_string += ";";

		res_2 = stmt_2->executeQuery(tmp_string);
		while (res_2->next())
		{
			tmp_person.bus_ids.push_back(res_2->getInt("bus_id"));
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

	str += "'";
	str += get_str_from_type(type);
	str += "'";

	str += ", ";
	str += "'";
	str += get_str_from_angle(angle);
	str += "'";

	str += ", ";
	str += std::to_string(occ_index);

	return str;
}

void DB_Connection::create_new_reg_en(int id, EntityType type, Angle angle, int occ_index)
{
	std::string str = get_basic_str("insert into Reg_En values(", id, type, angle, occ_index);
	str += ");";

	execute_stmt(str);
}

void DB_Connection::create_new_bus_stand_en(int id, EntityType type, Angle angle, int occ_index)
{
	std::string str = get_basic_str("insert into BusStand values(", id, type, angle, occ_index);
	str += ");";

	execute_stmt(str);
}

void DB_Connection::create_new_car_en(int id, EntityType type, Angle angle, int occ_index, int speed,
	float fuel_consumed, float miles_driven, int src_index, int des_index, int car_image_index)
{
	std::string str = get_basic_str("insert into Car values(", id, type, angle, occ_index);
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
	str += ", ";
	str += std::to_string(car_image_index);
	str += ");";

	execute_stmt(str);
}

void DB_Connection::create_new_traffic_light_en(int id, EntityType type, Angle angle, int occ_index,
	int time_delay, int junc_id, int pos_x, int pos_y)
{
	std::string str = get_basic_str("insert into TrafficLight values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(time_delay);
	str += ", ";
	str += std::to_string(junc_id);
	str += ", ";
	str += std::to_string(pos_x);
	str += ", ";
	str += std::to_string(pos_y);
	str += ");";

	execute_stmt(str);
}

void DB_Connection::create_new_person_en(int id, EntityType type, Angle angle, int occ_index,
	int image_index, int current_bst_id, int des_bst_id, float money_spent, std::vector<int> bus_ids)
{
	std::string str = get_basic_str("insert into Person values(", id, type, angle, occ_index);
	str += ", ";
	str += std::to_string(image_index);
	str += ", ";
	str += std::to_string(current_bst_id);
	str += ", ";
	str += std::to_string(des_bst_id);
	str += ", ";
	str += std::to_string(money_spent);
	str += ");";

	execute_stmt(str);

	// adding bus_ids to person_bus_stands
	for (unsigned int i = 0; i < bus_ids.size(); i++)
	{
		str = "insert into person_bus values(";
		str += std::to_string(id);
		str += ", ";
		str += std::to_string(bus_ids[i]);
		str += ");";

		execute_stmt(str);
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

	execute_stmt(str);

	// adding bus_stand_ids to bus_bus_stands
	for (unsigned int i = 0; i < bus_stand_ids.size(); i++)
	{
		str = "insert into bus_bus_stands values(";
		str += std::to_string(id);
		str += ", ";
		str += std::to_string(bus_stand_ids[i]);
		str += ");";

		execute_stmt(str);
	}
}

void DB_Connection::truncate_all()
{
	execute_stmt("truncate reg_en;");
	execute_stmt("truncate car;");
	execute_stmt("drop table person_bus;");
	execute_stmt("drop table bus_bus_stands;");
	execute_stmt("truncate bus;");
	execute_stmt("truncate person;");
	execute_stmt("truncate trafficlight;");
	execute_stmt("truncate busstand;");

	execute_stmt("create table bus_bus_stands(bus_id int, bus_stand_id int);");
	execute_stmt("alter table bus_bus_stands add foreign key(bus_id) references bus(id);");
	execute_stmt("alter table bus_bus_stands add foreign key(bus_stand_id) references busstand(id);");

	execute_stmt("create table person_bus(person_id int, bus_id int);");
	execute_stmt("alter table person_bus add foreign key(person_id) references person(id);");
	execute_stmt("alter table person_bus add foreign key(bus_id) references bus(id);");
}

void DB_Connection::generate_report(Simulation *sim)
{
	std::ofstream wr;
	wr.open("report.txt");
	if (!wr)
		return;

	std::string person_str_1 = update_money_spent(sim);
	std::string bus_cars_str_1 = update_miles_driven_and_fuel(sim);
	std::string extra_1 = get_extra_info();

	int total_roads = 0;
	int total_junctions = 0;
	int total_road_4_junctions = 0;
	int total_pavements = 0;
	int total_traffic_lights = 0;
	int total_people = 0;
	int total_cars = 0;
	int total_busses = 0;

	res = execute_query("select count(*) from reg_en where type = 'road';");
	res->next();
	total_roads = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from reg_en where type = 'junction';");
	res->next();
	total_junctions = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from reg_en where type = 'road_4_junc';");
	res->next();
	total_road_4_junctions = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from reg_en where type = 'pavement';");
	res->next();
	total_pavements = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from trafficlight;");
	res->next();
	total_traffic_lights = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from person;");
	res->next();
	total_people = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from car;");
	res->next();
	total_cars = res->getInt("count(*)");
	delete res;

	res = execute_query("select count(*) from bus;");
	res->next();
	total_busses = res->getInt("count(*)");
	delete res;

	wr << "\n--------------------- REPORT------------------------\n\n";

	wr << "total roads: " << total_roads << "\n";
	wr << "total junctions: " << total_junctions << "\n";
	wr << "total road_4_junctions: " << total_road_4_junctions << "\n";
	wr << "total pavements: " << total_pavements << "\n";
	wr << "total traffic_lights: " << total_traffic_lights << "\n";
	wr << "total people: " << total_people << "\n";
	wr << "total cars: " << total_cars << "\n";
	wr << "total busses: " << total_busses << "\n";

	wr << "\n";
	wr << person_str_1 << "\n";

	wr << "\n";
	wr << bus_cars_str_1 << "\n";

	wr << "\n";
	wr << extra_1 << "\n\n";

	wr << "\n--------------------- REPORT------------------------\n";

	wr.close();
}

std::string DB_Connection::get_in_quotes(int value)
{
	std::string tmp = "'";
	tmp += std::to_string(value);
	tmp += "'";

	return tmp;
}

std::string DB_Connection::update_money_spent(Simulation *sim)
{
	std::vector<Person*> people;
	for (unsigned int i = 0; i < sim->people.size(); i++)
		people.push_back((Person*)(sim->people[i]));

	std::string str;

	for (unsigned int i = 0; i < people.size(); i++)
	{
		std::string qe = "update person set money_spent = ";
		qe += std::to_string(people[i]->money_spent);
		qe = qe.substr(0, qe.size() - 5); // because of float(3,1) in sql
		qe += " where id = ";
		qe += std::to_string(people[i]->id);
		qe += ";";

		// std::cout << i << ": " << qe << std::endl;

		str += "Person with id ";
		str += std::to_string(people[i]->id);
		str += " has spent ";
		str += std::to_string(people[i]->money_spent);
		str += " Rs.";
		str += "\n";

		execute_stmt(qe);
	}

	return str;
}

std::string DB_Connection::get_extra_info()
{
	std::string str;

	res = execute_query("select avg(fuel_consumed) from car;");
	float avg_cars_fuel;
	res->next();
	avg_cars_fuel = res->getDouble(1);
	str = "Average fuel_consumed by all cars is ";
	str += std::to_string(avg_cars_fuel);
	str += " ltrs.\n";
	delete res;

	res = execute_query("select avg(fuel_consumed) from bus;");
	res->next();
	float avg_busses_fuel;
	avg_busses_fuel = res->getDouble(1);
	str += "Average fuel_consumed by all busses is ";
	str += std::to_string(avg_busses_fuel);
	str += " ltrs.\n";
	delete res;

	res = execute_query("select avg(money_spent) from person;");
	res->next();
	float avg_money_spent;
	avg_money_spent = res->getDouble(1);
	str += "Average money_spent by all people is ";
	str += std::to_string(avg_money_spent);
	str += " Rs.";
	delete res;

	return str;
}

std::string DB_Connection::update_miles_driven_and_fuel(Simulation *sim)
{
	std::vector<Car*> cars;
	std::vector<Bus*> busses;

	for (unsigned int i = 0; i < sim->bus_ens.size(); i++)
		busses.push_back((Bus*)(sim->bus_ens[i]));

	for (unsigned int i = 0; i < sim->car_ens.size(); i++)
		cars.push_back((Car*)(sim->car_ens[i]));

	std::string str;
	for (unsigned int i = 0; i < cars.size(); i++)
	{
		std::string qe;

		qe = "update car set fuel_consumed = ";
		qe += std::to_string(cars[i]->fuel_consumed);
		qe = qe.substr(0, qe.size() - 5); // because of float(3,1) in sql
		qe += " where id = ";
		qe += std::to_string(cars[i]->id);
		qe += ";";

		// std::cout << qe << std::endl;

		execute_stmt(qe);

		qe = "update car set miles_driven = ";
		qe += std::to_string(cars[i]->miles_driven);
		qe = qe.substr(0, qe.size() - 5); // because of float(3,1) in sql
		qe += " where id = ";
		qe += std::to_string(cars[i]->id);
		qe += ";";

		// std::cout << qe << std::endl;

		str += "Car with id ";
		str += std::to_string(cars[i]->id);
		str += " has consumed ";
		str += std::to_string(cars[i]->fuel_consumed);
		str += " ltrs of fuel and has travelled ";
		str += std::to_string(cars[i]->miles_driven);
		str += " miles.\n";

		execute_stmt(qe);
	}

	for (unsigned int i = 0; i < busses.size(); i++)
	{
		std::string qe;

		qe = "update bus set fuel_consumed = ";
		qe += std::to_string(busses[i]->fuel_consumed);
		qe = qe.substr(0, qe.size() - 5); // because of float(3,1) in sql
		qe += " where id = ";
		qe += std::to_string(busses[i]->id);
		qe += ";";

		execute_stmt(qe);
		// std::cout << qe << std::endl;

		qe = "update bus set miles_driven = ";
		qe += std::to_string(busses[i]->miles_driven);
		qe = qe.substr(0, qe.size() - 5); // because of float(3,1) in sql
		qe += " where id = ";
		qe += std::to_string(busses[i]->id);
		qe += ";";

		execute_stmt(qe);
		// std::cout << qe << std::endl;

		str += "Bus with id ";
		str += std::to_string(busses[i]->id);
		str += " has consumed ";
		str += std::to_string(busses[i]->fuel_consumed);
		str += " ltrs of fuel and has travelled ";
		str += std::to_string(busses[i]->miles_driven);
		str += " miles.\n";
	}

	return str;
}

sql::ResultSet* DB_Connection::execute_query(std::string str)
{
	stmt = con->createStatement();
	sql::ResultSet *tmp_res = stmt->executeQuery(str);
	delete stmt;

	return tmp_res;
}

void DB_Connection::execute_stmt(std::string str)
{
	stmt = con->createStatement();
	stmt->execute(str);

	delete stmt;
}

void DB_Connection::destroy()
{
	delete con;
}
