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
Table::Table(Table &other): capacity(other.getCapacity()), open(other.isOpen()) {
    customersList=other.getCustomers();
    orderList=other.getOrders();
}
// Table copy operator=
Table& Table::operator=(const Table &other) {
    if(this!=&other) {
        clear();
        capacity=other.capacity;
        open=other.open;
        customersList=other.customersList;
        orderList=other.orderList;
    }
    return *this;
}

// Table move operator=
Table& Table::operator=(Table &&other) {
    if(this!=&other) {
        clear();
        capacity=other.capacity;
        open=other.open;
        customersList=other.customersList;
        orderList=other.orderList;
        other.open=false;
        other.capacity=0;
        other.orderList.clear();
        other.orderList.shrink_to_fit();
        other.customersList.clear();
        other.customersList.shrink_to_fit();
    }
    return *this;
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
    {
        delete customersList.at(i);
        customersList.at(i) = nullptr;
    }
    customersList.shrink_to_fit();
    open=false;
}
