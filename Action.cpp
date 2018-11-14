
//
// Created by simo on 11/9/18.
//

#include "Action.h"


using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

BaseAction:: BaseAction():status(PENDING){}

void BaseAction::complete() { status = COMPLETED; }
void BaseAction::setInputString(std::string str) {inputString=str;}
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
    if(restaurant.getTables().size()>=tableId&&((restaurant.getTables().at(tableId).getCapacity()-restaurant.getTables().at(tableId).getCustomers().size())>=customers.size())&&restaurant.getTables().at(tableId).isOpen()!=true){
       restaurant.getTables().at(tableId).openTable();    ////////////////on the ears
        for(int i=0;i<customers.size();i++)
            restaurant.getTables().at(tableId).getCustomers().push_back(customers.at(i));
    }
    else
        error("Table does not exist or is already open");
    }
void setInputStr(string args){
    str=args;
}
string OpenTable::toString(){
        return str;
    }

// ---------------------------- Action: Move Customer -----------------------------------------

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src),dstTable(dst),id(customerId), str("move") {}
void MoveCustomer::act(Restaurant &restaurant) {
    if(restaurant.getTables().size()<max(srcTable, dstTable)||!restaurant.getTables().at(srcTable).isOpen()||!restaurant.getTables().at(dstTable).isOpen()||restaurant.getTables().at(dstTable).getCapacity()<=restaurant.getTables().at(dstTable).getCustomers().size())
    {
        error("Cannot move customer");
        cout <<"Cannot move customer";
    }
    else {
        Customer *cust=restaurant.getTables().at(srcTable).getCustomer(id);
        restaurant.getTables().at(dstTable).addCustomer(cust);
        restaurant.getTables().at(srcTable).removeCustomer(id);
        for (int i=0;i<restaurant.getTables().at(srcTable).getOrders().size();i++) {
            if(restaurant.getTables().at(srcTable).getOrders().at(i).first==id) {
                restaurant.getTables().at(dstTable).getOrders().push_back(restaurant.getTables().at(srcTable).getOrders().at(i));
                restaurant.getTables().at(srcTable).getOrders().erase(restaurant.getTables().at(srcTable).getOrders().begin()+i);
            }
        }
        if(restaurant.getTables().size()==0)
            restaurant.getTables().at(srcTable).closeTable();
    }
}
void setInputStr(string args){
    str=args;
}
std::string MoveCustomer::toString() const {return str;}

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
void setInputStr(string args){
    str=args;
}
std::string Close::toString() const {return str;}

// ---------------------------- Action: CloseAll  -----------------------------------------

CloseAll::CloseAll(){}
void CloseAll::act(Restaurant &restaurant) {
    for (int i=0; i<restaurant.getTables().size();i++){
        cout <<"Table "<<i<<"is was closed. Bill is "<<restaurant.getTables().at(i).getBill()<<"NIS /n";
        restaurant.getTables().at(i).closeTable();
    }
    restaurant.~Restaurant();
}
std::string CloseAll::toString() const {return "Close All";}

// ---------------------------- Action: PrintActionsLog  -----------------------------------------

PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog(Restaurant &restaurant){
    for(int i=0;i<restaurant.getActionsLog().size();i++)
        cout << restaurant.getActionsLog().at(i)->toString()<<endl<<convertToString(restaurant.getActionsLog().at(i));
}