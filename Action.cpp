
//
// Created by simo on 11/9/18.
//

#include "Action.h"
#include "Dish.h"
#include "Customer.h"
#include "Restaurant.h"


using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

BaseAction:: BaseAction():status(PENDING) {}

void BaseAction::complete() { status = COMPLETED; }

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
    errorMsg = errorMsg;
    cout << errorMsg << endl;
}

// ---------------------------- Action: Open Table -----------------------------------------

//Constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), str("open") {
    for(int i=0; i<customersList.size();i++){
        customers.push_back(customersList.at(i)->clone);
    }
}

//Copy Constructor
OpenTable::OpenTable(OpenTable &other):tableId(other.tableId), str("open") {
    for(int i=0; i<other.customers.size();i++)
        customers.push_back(other.customers.at(i)->clone);
}

//Copy Operator=
OpenTable& OpenTable::operator=(const OpenTable &other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&tableId);
        *ptr=other.tableId;
        //copying other's customers list
        for(int i=0; i<other.customers.size();i++)
            customers.push_back(other.customers.at(i)->clone);
    }
    return *this;
}

//Move Operator=
OpenTable& OpenTable::operator=(OpenTable &&other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&tableId);
        *ptr=other.tableId;
        //copying other's customers list
        for(int i=0; i<other.customers.size();i++)
            customers.push_back(other.customers.at(i)->clone);
        other.clear();
    }
    return *this;
}

//destructor
OpenTable::~OpenTable() {
    int *ptr;
    ptr=(int*)(&tableId);
    *ptr=0;
    clear();
}
void OpenTable::clear(){
    for(int i=0;i<customers.size();i++)
        delete customers.at(i);
    customers.clear();
    customers.shrink_to_fit();
}

void OpenTable::act(Restaurant &restaurant) {
    if(isError(restaurant))
        error("Table does not exist or is already open");
    else {
        restaurant.getTable(tableId)->openTable();
        for (int i = 0; i < customers.size(); i++)
            restaurant.getTable(tableId)->getCustomers().push_back(customers.at(i));
        complete();
    }
}

// the function returns true if the table cannot be opened
bool OpenTable::isError(Restaurant& restaurant) {
    bool p1 = restaurant.getNumOfTables() <= tableId;
    int capacity=restaurant.getTable(tableId)->getCapacity();
    int currentCusts=restaurant.getTable(tableId)->getCustomers().size();
    int newCusts=customers.size();
    bool p2 = capacity - currentCusts < newCusts;
    bool p3=restaurant.getTable(tableId)->isOpen();
    return p1 | p2 | p3;
}

void OpenTable::setInputStr(string args) { str=args; }

string OpenTable::toString() const{
    return str;
}

// ------------------------------------------- Action: Order
//constructor
Order::Order(int id):tableId(id),str("order") {}

//Copy Constructor
Order::Order(Order &other):tableId(other.tableId), str("order") {}

//Copy Operator=
Order& Order::operator=(const Order &other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&tableId);
        *ptr=other.tableId;
    }
    return *this;
}

//Move Operator=
Order& Order::operator=(Order &&other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&tableId);
        *ptr=other.tableId;
        other.clear();
    }
    return *this;
}

//destructor
Order::~Order() { clear();}
void Order::clear(){
    int *ptr;
    ptr=(int*)(&tableId);
    *ptr=0;
    str="";
}

void Order::act(Restaurant &restaurant) {
    if(!restaurant.getTable(tableId)->isOpen() | restaurant.getNumOfTables()<=tableId)
        error("Table does not exist or is not open");
    else {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
}
void Order::setInputStr(string args) { str=args; }

std::string Order::toString() const { return str; }

// ---------------------------- Action: Move Customer -----------------------------------------

//constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src),dstTable(dst),id(customerId),
                                                              str("move") {}

//Copy Constructor
MoveCustomer::MoveCustomer(const MoveCustomer &other):id(other.id), srcTable(other.srcTable), dstTable(other.dstTable),
                                                              str("move") {}

//Copy Operator=
MoveCustomer& MoveCustomer::operator=(const MoveCustomer &other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&id);
        *ptr=other.id;
        ptr=(int*)(&srcTable);
        *ptr=other.srcTable;
        ptr=(int*)(&dstTable);
        *ptr=other.dstTable;
    }
    return *this;
}

//Move Operator=
MoveCustomer& MoveCustomer::operator=(MoveCustomer &&other){
    if(this!=&other){
        //deleting the old data
        clear();
        /*changing const int, needs to be checked*/
        int *ptr;
        ptr=(int*)(&id);
        *ptr=other.id;
        ptr=(int*)(&srcTable);
        *ptr=other.srcTable;
        ptr=(int*)(&dstTable);
        *ptr=other.dstTable;
        other.clear();
    }
    return *this;
}

//destructor
MoveCustomer::~MoveCustomer() { clear();}
void MoveCustomer::clear(){
    int *ptr;
    ptr=(int*)(&srcTable);
    *ptr=0;
    ptr=(int*)(&dstTable);
    *ptr=0;
    ptr=(int*)(&id);
    *ptr=0;
    str="";
}

void MoveCustomer::act(Restaurant &restaurant) {
    if(isError(restaurant))
        error("Cannot move customer");
    else {
        Table *srcT=restaurant.getTable(srcTable);
        Table *dstT=restaurant.getTable(dstTable);
        Customer *cust=srcT->getCustomer(id);
        dstT->addCustomer(cust);
        srcT->removeCustomer(id);
        for (int i=0;i < srcT->getOrders().size();i++) {        // adding the customer's orders to the dst table
            if(srcT->getOrders().at(i).first==id)
                dstT->getOrders().push_back(srcT->getOrders().at(i));
        }
        srcT->getOrders()=removeOrders(srcT->getOrders(), id); // removing the customers orders from the src table
        if(srcT->getCustomers().size()==0)
            srcT->closeTable();
        complete();
        srcT=nullptr;
        dstT=nullptr;
    }
}
// the function returns a vector without the customer's orders
vector<OrderPair> MoveCustomer::removeOrders(vector<OrderPair> &orders, int id) {
    vector<OrderPair> afterRemoval;
    for(int i=0;i<orders.size();i++)
        if(orders.at(i).first!=id)
            afterRemoval.push_back(orders.at(i));
    return afterRemoval;
}

// the function returns true if the customer cannot be moved;
bool MoveCustomer::isError(Restaurant &restaurant) {
    bool p1=restaurant.getNumOfTables()<=max(srcTable, dstTable);
    bool p2=!restaurant.getTable(srcTable)->isOpen();
    bool p3=!restaurant.getTable(dstTable)->isOpen();
    bool p4=restaurant.getTable(dstTable)->getCapacity() == restaurant.getTable(dstTable)->getCustomers().size();
    return p1 | p2 | p3 | p4;
}

void MoveCustomer::setInputStr(string args) { str=args; }

std::string MoveCustomer::toString() const { return str; }

// ---------------------------- Action: Close  -----------------------------------------

//Constructor
Close::Close(int id): tableId(id), str("close"){}

void Close::act(Restaurant &restaurant) {
    if(restaurant.getNumOfTables()<tableId || !restaurant.getTable(tableId)->isOpen())
        cout << "Table does not exist or is not open";
    else{
        cout << "Table " << tableId << " was closed. Bill is " << restaurant.getTable(tableId)->getBill() << "NIS";
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}
void Close::setInputStr(string args) { str=args; }

std::string Close::toString() const { return str; }

// ---------------------------- Action: CloseAll  -----------------------------------------

CloseAll::CloseAll()=default;

void CloseAll::act(Restaurant &restaurant) {
    for (int i=0; i<restaurant.getNumOfTables();i++){
        cout << "Table " << i << " was closed. Bill is " << restaurant.getTable(i)->getBill() << "NIS /n";
        restaurant.getTable(i)->closeTable();
    }
    restaurant.~Restaurant();
}

std::string CloseAll::toString() const { return "Close All"; }

// ------------------------------------------  PrintMenu  ------------------------------------------------------
PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    for(auto i=restaurant.getMenu().begin(); i!=restaurant.getMenu().end();i++)
        cout << i->toString();
    complete();
}

void PrintMenu::setInputStr(string args) { str=args; }

string PrintMenu::toString() const { return str; }

// ------------------------------------------  PrintTableStatus  ------------------------------------------------------
PrintTableStatus::PrintTableStatus(int id):tableId(id) {}


void PrintTableStatus::act(Restaurant &restaurant) {
    cout << "Table " << tableId << " Status: ";
    if(restaurant.getTable(tableId)->isOpen()) {
        cout << "open" << endl;
        cout << "Customers:" << endl;
        for (auto i = restaurant.getTable(tableId)->getCustomers().begin();
                  i != restaurant.getTable(tableId)->getCustomers().end(); i++)
            cout << (*i)->getId() << " " << (*i)->getName() << endl;
        cout << "Orders:" << endl;
        for (auto i = restaurant.getTable(tableId)->getOrders().begin();
                  i != restaurant.getTable(tableId)->getOrders().end(); i++)
            cout << i->second.getName() << " " << i->second.getPrice() << " " << i->first << endl;
        cout << restaurant.getTable(tableId)->getBill() << endl;
    }
    else
        cout << "closed" << endl;
    complete();
}
// ---------------------------- Action: PrintActionsLog  -----------------------------------------

//Constructor
PrintActionsLog::PrintActionsLog():str("print actions log") {}

void PrintActionsLog::act(Restaurant &restaurant){
    for(int i=0;i<restaurant.getActionsLog().size();i++) {
        cout << restaurant.getActionsLog().at(i)->toString() << " ";
        cout << convertToString(getStatus());
        if(getStatus()==ERROR)
            cout << ":" << getErrorMsg();
        cout << endl;
    }
    complete();
}

//void PrintActionsLog::setInputStr(string args) { str=args; } //not sure if necessary

std::string PrintActionsLog::toString() const { return str; }

void PrintTableStatus::setInputStr(string args) { str=args; }

std::string PrintTableStatus::toString() const { return str; }

// ------------------------------------------  BackupRestaurant  ------------------------------------------------------
BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant) {
    extern Restaurant* backup;
    backup=new Restaurant(restaurant);
    complete();
}

void BackupRestaurant::setInputStr(string args) { str=args; }

std::string BackupRestaurant::toString() const { return str; }

// ------------------------------------------  RestoreRestaurant  ------------------------------------------------------
RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    extern Restaurant* backup;
    if(backup==nullptr)
        error("No backup available");
    else {
        restaurant = *backup;
        complete();
    }
}

void RestoreResturant::setInputStr(string args) { str=args; }

std::string RestoreResturant::toString() const { return str; }