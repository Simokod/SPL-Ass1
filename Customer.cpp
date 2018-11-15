//
// Created by barsimo on 11/6/18.
//

#include "Customer.h"
using namespace std;

    Customer::Customer(string c_name, int c_id): name(c_name), id(c_id){}

    int Customer::getId() const {
        const int &_id=id;
        return _id;
    }

    string Customer::getName() const {
        const string &_name=name;
        return _name;
    }

//----------------------------------------------Vegetarian Customer--------------------------------------
    VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name, id) {}

    std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;
        // adding the first vegeterian dish to the order
        for (auto i = menu.begin(); i != menu.end(); ++i)
            if(i->getType()==VEG) {
                orders.push_back(i->getId());
                break;
            }
        // adding the most expensive beverage dish to the order
        for (auto i = menu.end(); i != menu.end(); --i)
            if(i->getType()==BVG) {
                orders.push_back(i->getId());
                break;
            }
        return orders;
    }

    std::string VegetarianCustomer::toString() const {
        return getName()+",veg";
    }
//----------------------------------------------Cheap Customer--------------------------------------
    CheapCustomer::CheapCustomer(string name, int id): Customer(name, id), ordered(false) {}

    std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;
        if(!ordered)
        {
            orders.push_back(menu.begin()->getId());
            ordered = true;
        }
        return orders;
    }

    std::string CheapCustomer::toString() const {
        return getName()+",chp";
    }

//----------------------------------------------Spicy Customer--------------------------------------
    SpicyCustomer::SpicyCustomer(string name, int id): Customer(name, id), ordered(false) {}

    std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;
        if(!ordered)
            for(int i=menu.size();i>=0;i--)
                if(menu.at(i).getType()==SPC)
                {
                    orders.push_back(menu.at(i).getId());
                    break;
                }
        else
            for(int i=0;i<menu.size();i++)
                if(menu.at(i).getType()==BVG)
                {
                    orders.push_back(menu.at(i).getId());
                    break;
                }
        return orders;
    }

    string SpicyCustomer::toString() const {
        return getName()+",spc";
    }

//----------------------------------------------Alchoholic Customer--------------------------------------
    AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name, id), lastOrder(-1) {}

    std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;
        lastOrder++;
        while(lastOrder<menu.size())
        {
            if(menu.at(lastOrder).getType()==ALC)
            {
                orders.push_back(menu.at(lastOrder).getId());
                break;
            }
            lastOrder++;
        }
        return orders;
    }

    string AlchoholicCustomer::toString() const {
        return getName()+",alc";
    }