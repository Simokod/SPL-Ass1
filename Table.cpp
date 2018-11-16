//
// Created by simo on 11/7/18.
//

#include <vector>
#include "Table.h"
#include "Customer.h"
#include <iostream>
using namespace std;

// Table constructor
Table::Table(int t_capacity): capacity(t_capacity), open(false) {}

// Table copy constructor
Table::Table(const Table &other): capacity(other.getCapacity()), open(other.open) {
    // copying other customersList
    for (int i = 0; i < getCustomers().size(); i++)
        customersList.push_back(other.customersList.at(i)->clone());
    // copying other orderList
    for (int i = 0; i < other.orderList.size(); i++)
        orderList.push_back(other.orderList.at(i));
}

// Table move constructor
Table::Table(Table &&other): capacity(other.capacity), open(other.open) {
    for(int i=0;i<other.customersList.size();i++)
        customersList.push_back(other.customersList.at(i));
    for(int i=0;i<other.orderList.size();i++)
        orderList.push_back(other.orderList.at(i));
    other.clear();
}

// Table copy operator=
Table& Table::operator=(const Table &other) {
    if(this!=&other) {
        clear();
        capacity=other.capacity;
        open=other.open;
        // deleting this costumersList
        for(int i=0;i<customersList.size();i++)
            delete customersList.at(i);
        // copying other costumersList
        for(int i=0;i<getCustomers().size();i++) {
            customersList.push_back(other.customersList.at(i)->clone());
        }
        // copying other orderList
        for(int i=0;i<other.orderList.size();i++)
            orderList.push_back(other.orderList.at(i));
    }
    return *this;
}

// Table move operator=
Table& Table::operator=(Table &&other) {
    if(this!=&other) {
        clear();
        capacity=other.capacity;
        open=other.open;
        // copying other customersList
        for(int i=0;i<getCustomers().size();i++) {
            customersList.push_back(other.customersList.at(i)->clone());
        }
        // copying other orderList
        for(int i=0;i<other.orderList.size();i++)
            orderList.push_back(other.orderList.at(i));
        other.clear();
    }
    return *this;
}

int Table::getCapacity() const { return capacity; }

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

std::vector<Customer*>& Table::getCustomers() { return customersList; }

std::vector<OrderPair>& Table::getOrders() { return orderList; }

void Table::removeCustomer(int id) {
    for(int i=0;i<customersList.size();i++)
        if(customersList.at(i)->getId()==id)
            customersList.erase(customersList.begin()+i);
}
// returns the customer with the matching id
Customer* Table::getCustomer(int id) {
    for(int i=0;i<customersList.size();i++)
        if(customersList.at(i)->getId()==id)
            return customersList.at(i);
}
// takes an order from each customer on the table
void Table::order(const std::vector<Dish> &menu) {
    for(int i=0;i<customersList.size();i++){
        vector<int> cusOrders=customersList.at(i)->order(menu);
        for(int k=0;k<cusOrders.size();k++)     // going over the customer's orders
            for(int j=0;j<menu.size();j++)      // finding dish by Id
                if(cusOrders.at(k)==menu.at(j).getId()) {
                    orderList.push_back(*new OrderPair(customersList.at(i)->getId(), menu.at(j)));
                    cout << customersList.at(i)->getName() << " ordered " << orderList.back().second.getName() << endl;
                }
    }
}
void Table::openTable() { open = true; }
void Table::closeTable() {clear();}
bool Table::isOpen() { return open; }

int Table::getBill() {
    int bill=0;
    for(int i=0;i<orderList.size();i++)
        bill+=orderList.at(i).first;
    return bill;
}
// Table destructor
Table::~Table() {
    capacity=0;
    clear();
}

void Table::clear() {
    orderList.clear();
    orderList.shrink_to_fit();
    for(int i=0;i<customersList.size();i++)
        delete customersList.at(i);
    customersList.clear();
    customersList.shrink_to_fit();
    open=false;
}