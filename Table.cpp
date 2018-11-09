//
// Created by simo on 11/7/18.
//

#include "Table.h"

using namespace std;


Table::Table(int t_capacity) {
    capacity = t_capacity;
    open = false;
}

int Table::getCapacity() const { return capacity; }

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

std::vector<Customer*>& Table::getCustomers() { return customersList; }

std::vector<OrderPair>& Table::getOrders() { return orderList; }

// removes a customer from the table, does not take care of orders****************
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

void Table::openTable() { open = true; }
void Table::closeTable() { open = false; }
bool Table::isOpen() { return open; }

int Table::getBill() {
    int bill=0;
    for(int i=0;i<orderList.size();i++)
        bill+=orderList.at(i).first;
    return bill;
}

Table::~Table() { clear(); }

void Table::clear() {
}