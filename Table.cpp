//
// Created by simo on 11/7/18.
//

#include <vector>
#include "Table.h"
#include "Customer.h"
using namespace std;

// Table constructor
Table::Table(int t_capacity): capacity(t_capacity), open(false) {}

// Table copy constructor
Table::Table(Table &other): capacity(other.capacity), open(other.open) {
    customersList=other.customerList;
    orderList=other.orderList;
}
// Table move constructor
Table::Table(Table &&other): capacity(other.getCapacity()), open(other.isOpen()) {
    customersList=other.customersList;
    orderList=other.orderList;
    other.customersList= nullptr;
    other.orderList= nullptr;
    other.open=false;
    other.capacity=0;
}
// Table copy assignment operator
Table& Table::operator=(const Table &other) {
    if(this!=other) {
        clear();
        capacity=other.capacity;
        open = other.open;
        customersList=other.customersList;
        orderList=other.orderList;
    }
    return *this;
}
// Table move assignment operator
Table& Table::operator=(const Table &&other) {
    if(this!= &other)
    {
        clear();
        open=other.open;
        capacity=other.capacity;
        customersList=other.customersList;
        orderList=other.orderList;
        other.customersList= nullptr;
        other.orderList= nullptr;
        other.open=false;
        other.capacity=0;
    }
    return  *this;
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
// Table destructor
Table::~Table() { clear(); }

private void Table::clear(){
    for(int i=0;i<customersList.size();i++)
        delete customersList.at(i);
    customerList.clear();
    for(int i=0;i<orderList.size();i++)
        delete orderList.at(i);
    orderList.clear();
    capacity=0;
    open=false;
    customerList= nullptr;
    orderList= nullptr;
}