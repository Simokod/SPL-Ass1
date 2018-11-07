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
        string s1=getName();
        string s2=", "+getId();
        string s3=", Vegeterian Customer";
        return s1+s2+s3;
    }
//----------------------------------------------Cheap Customer--------------------------------------
    CheapCustomer::CheapCustomer(string name, int id): Customer(name, id) {}

    std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;
        orders.push_back(menu.begin()->getId());
        return orders;
    }

    std::string CheapCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Cheap Customer";
        return s1+s2+s3;
    }

//----------------------------------------------Spicy Customer--------------------------------------
    SpicyCustomer::SpicyCustomer(string name, int id): Customer(name, id) {}

    std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
        vector<int> orders;

    }

    string SpicyCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Spicy Customer";
        return s1+s2+s3;
    }

//----------------------------------------------Alchoholic Customer--------------------------------------
    AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name, id) {}



    string AlchoholicCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Alchoholic Customer";
        return s1+s2+s3;
    }






















