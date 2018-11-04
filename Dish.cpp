#include "Dish.h"
#include <string>

using namespace std;

enum DishType{
    VEG, SPC, BVG, ALC
};

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type);
{
	id=d_id;
	name=d_name;
	price=d_price;
	type=d_type;
}

int Dish::getId(){ return id; }
string Dish::getName(){ return name; }
int Dish::getPrice(){ return price; }
DishType Dish::getType(){ return type; }