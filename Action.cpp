
//
// Created by simo on 11/9/18.
//

#include "Action.h"


using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

BaseAction:: BaseAction():status(PENDING){}

void BaseAction::complete() { status = COMPLETED; }

std::string BaseAction::getErrorMsg() const { return errorMsg; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    errorMsg=errorMsg;
    cout << errorMsg << endl;
}

// ---------------------------- Action: Open Table -----------------------------------------
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), customers(customersList) {}
void OpenTable::act(Restaurant &restaurant) {
    if(restaurant.getTables().size()>=tableId&&((restaurant.getTables().at(tableId).getCapacity()-restaurant.getTables().at(tableId).getCustomers().size())>=customers.size())&&restaurant.getTables().at(tableId).isOpen()!=true){
        restaurant.getTables().at(tableId).openTable();    ////////////////on the ears
        for(int i=0;i<customers.size();i++) {
            restaurant.getTables().at(tableId).getCustomers().push_back(customers.at(i));
        }
    }
    else
        cout <<"Table does not exist or is already open";
}
string OpenTable::toString(){
    string str="This action opens a table by a given id, and adds a customers list to the table";
}
// ------------------------------------------  PrintMenu  ------------------------------------------------------
PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for(auto i=restaurant.getMenu().begin(); i!=restaurant.getMenu().end();i++)
        cout << i->toString();
}

string PrintMenu::toString() const { return str; }

// ------------------------------------------  PrintTableStatus  ------------------------------------------------------
PrintTableStatus::PrintTableStatus(int id):tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) {
    cout << "Table " << tableId << " Status: " << endl;
    Table *t=restaurant.getTable(tableId);
    if(t->isOpen()) {
        cout << "open";
        cout << "Customers:" << endl;
        for (auto i = t->getCustomers().begin(); i != t->getCustomers().end(); i++)
            cout << (*i)->getId() << " " << (*i)->getName() << endl;
        cout << "Orders:" << endl;
        for (auto i = t->getOrders().begin(); i != t->getOrders().end(); i++)
            cout << i->second.getName() << " " << i->second.getPrice() << " " << i->first << endl;
        cout << t->getBill() << endl;
    }
    else
        cout << "closed" << endl;
}

std::string PrintTableStatus::toString() const { return str; }

// ------------------------------------------  BackupRestaurant  ------------------------------------------------------
BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    backup=new Restaurant(restaurant);
}

std::string BackupRestaurant::toString() const { return str; }

// ------------------------------------------  BackupRestaurant  ------------------------------------------------------
RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup==nullptr)
        error("No backup available");
    else
        restaurant=*backup;
}

std::string RestoreResturant::toString() const { return str; }