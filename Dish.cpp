#include "Dish.h"
#include <string>

using namespace std;


Dish::Dish(int d_id, string d_name, int d_price, DishType d_type): id(d_id), name(d_name), price(d_price), type(d_type){}

    int Dish::getId() const {
        const int &_id=id;
        return _id;
    }
    string Dish::getName() const {
        const string &_name=name;
        return _name;
    }
    int Dish::getPrice() const {
        const int &_price=price;
        return _price;
    }
    DishType Dish::getType() const {
        const DishType &_type=type;
        return _type;
    }