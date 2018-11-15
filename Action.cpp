
//
// Created by simo on 11/9/18.
//

#include "Action.h"


using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

BaseAction:: BaseAction():status(PENDING){}

void BaseAction::complete() { status = COMPLETED; }
void BaseAction::setInputStr(std::string str) {inputString=str;}
string BaseAction::getErrorMsg() const { return errorMsg; }
string BaseAction::convertToString(ActionStatus stat) {
    switch(stat)
    {
        case PENDING: return "pending";
        case COMPLETED: return "completed";
        case ERROR: return "error";
    }
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    errorMsg=errorMsg;
    cout<<errorMsg<<endl;
}

// ---------------------------- Action: Open Table -----------------------------------------
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), customers(customersList), str("open") {}
void OpenTable::act(Restaurant &restaurant) {
    if(isError(restaurant))
        error("Table does not exist or is already open");
    else {
        restaurant.getTables().at(tableId)->openTable();
        for (int i = 0; i < customers.size(); i++)
            restaurant.getTables().at(tableId)->getCustomers().push_back(customers.at(i));
    }
}
void OpenTable::setInputStr(string args){
    str=args;
}
string OpenTable::toString() const{
    return str;
}
// the function returns true if the table cannot be opened
bool OpenTable::isError(Restaurant& restaurant) {
    bool p1 = restaurant.getTables().size() <= tableId;
    int capacity=restaurant.getTable(tableId)->getCapacity();
    int currentCusts=restaurant.getTable(tableId)->getCustomers().size();
    int newCusts=customers.size();
    bool p2 = capacity - currentCusts < newCusts;
    bool p3=restaurant.getTable(tableId)->isOpen();
    return p1 | p2 | p3;
}

// ------------------------------------------- Action: Order
Order::Order(int id):tableId(id) {}

void Order::act(Restaurant &restaurant) {
    if(!restaurant.getTable(tableId)->isOpen() | restaurant.getNumOfTables()<=tableId)
        error("Table does not exist or is not open");
    else
        restaurant.getTable(tableId)->order(restaurant.getMenu());
}

std::string Order::toString() const { return str; }

void Order::setInputStr(string args) { str=args; }

// ---------------------------- Action: Move Customer -----------------------------------------

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src),dstTable(dst),id(customerId),
                                                              str("move") {}

void MoveCustomer::act(Restaurant &restaurant) {
    if(isError(restaurant))
        error("Cannot move customer");
    else {
        Customer *cust=restaurant.getTable(srcTable)->getCustomer(id);
        restaurant.getTable(dstTable)->addCustomer(cust);
        restaurant.getTable(srcTable)->removeCustomer(id);
        for (int i=0;i<restaurant.getTable(srcTable)->getOrders().size();i++) {
            if(restaurant.getTable(srcTable)->getOrders().at(i).first==id) {
                restaurant.getTable(dstTable)->getOrders().push_back(restaurant.getTable(srcTable)->getOrders().at(i));
                restaurant.getTable(srcTable)->getOrders().erase(restaurant.getTable(srcTable)->getOrders().begin()+i);
            }
        }
        if(restaurant.getTables().size()==0)
            restaurant.getTable(srcTable)->closeTable();
    }
}

void MoveCustomer::setInputStr(string args){ str=args; }

// the function returns true if the customer cannot be moved;
std::string MoveCustomer::toString() const {return str;}
bool MoveCustomer::isError(Restaurant &restaurant) {
    bool p1=restaurant.getTables().size()<=max(srcTable, dstTable);
    bool p2=!restaurant.getTable(srcTable)->isOpen();
    bool p3=!restaurant.getTable(dstTable)->isOpen();
    bool p4=restaurant.getTable(dstTable)->getCapacity() == restaurant.getTable(dstTable)->getCustomers().size();
    return p1 | p2 | p3 | p4;
}

std::string MoveCustomer::toString() const { return str; }

// ---------------------------- Action: Close  -----------------------------------------

Close::Close(int id): tableId(id), str("close"){}
void Close::act(Restaurant &restaurant) {
    if(restaurant.getTables().size()<tableId||!restaurant.getTables().at(tableId).isOpen())
        cout <<"Table does not exist or is not open";
    else{
        cout <<"Table "<<tableId<<"is was closed. Bill is "<<restaurant.getTables().at(tableId).getBill()<<"NIS";
        restaurant.getTables().at(tableId).closeTable();
    }
}
void Close::setInputStr(string args) { str=args; }

std::string Close::toString() const { return str; }

// ---------------------------- Action: CloseAll  -----------------------------------------

CloseAll::CloseAll(){}

void CloseAll::act(Restaurant &restaurant) {
    for (int i=0; i<restaurant.getTables().size();i++){
        cout <<"Table "<<i<<"is was closed. Bill is "<<restaurant.getTables().at(i).getBill()<<"NIS /n";
        restaurant.getTables().at(i).closeTable();
    }
    restaurant.~Restaurant();
}

std::string CloseAll::toString() const { return "Close All"; }

// ---------------------------- Action: PrintActionsLog  -----------------------------------------

PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog::act(Restaurant &restaurant){
    for(int i=0;i<restaurant.getActionsLog().size();i++)
        cout << restaurant.getActionsLog().at(i)->toString()<<endl<<convertToString(restaurant.getActionsLog().at(i));
}
