#include "Dish.h"
#include <string>


    Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price),
                                                                        type(d_type) {}

    int Dish::getId() const {
        return id;
    }

    std::string Dish::getName() const {
        return name;
    }

    int Dish::getPrice() const {
        return price;
    }

    DishType Dish::getType() const {
        return type;
    }

    std::string Dish::toString() {
        std::string s=name+" ";
        if(type==VEG)
            s+="VEG ";
        if(type==SPC)
            s+="SPC ";
        if(type==BVG)
            s+="BVG ";
        if(type==ALC)
            s+="ALC ";
        s+=std::to_string(price);
        return s;
    }