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
