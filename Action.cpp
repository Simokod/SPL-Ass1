
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


}