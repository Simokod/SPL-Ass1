//
// Created by simo on 11/9/18.
//

#include "Action.h"
#include "Dish.h"
#include "Customer.h"
#include "Restaurant.h"


using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

BaseAction::BaseAction():errorMsg(""),status(PENDING) {}

void BaseAction::complete() { status = COMPLETED; }

string BaseAction::getErrorMsg() const { return errorMsg; }

string BaseAction::convertToString(ActionStatus stat) const {
    switch(stat)
    {
        case PENDING: return "Pending";
        case COMPLETED: return "Completed";
        case ERROR: return "Error";
    }
    return "";
}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    cout << errorMsg << endl;
}

BaseAction::~BaseAction()=default;
// ---------------------------- Action: Open Table -----------------------------------------

//Constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), str("open"), customers() {
    for(unsigned long i=0; i<customersList.size();i++){
        customers.push_back(customersList.at(i));
    }
}

//Copy Constructor
OpenTable::OpenTable(const OpenTable &other):tableId(other.tableId), str(other.str), customers() {
    for(unsigned long i=0; i<other.customers.size();i++)
        customers.push_back(other.customers.at(i)->clone());
}

// Move Operator
OpenTable::OpenTable(OpenTable &&other): tableId(other.tableId), str(other.str), customers() {
    //copying other's customers list
    for(unsigned long i=0; i<other.customers.size();i++)
        customers.push_back(other.customers.at(i)->clone());
    other.clear();
}
//destructor
OpenTable::~OpenTable() { clear(); }

void OpenTable::clear(){
    for(unsigned long i=0;i<customers.size();i++)
        delete customers.at(i);
    customers.clear();
}

void OpenTable::act(Restaurant &restaurant) {
    if(isError(restaurant)) {
        error("Table does not exist or is already open");
        clear();
    }
    else {
        restaurant.getTable(tableId)->openTable();
        for (unsigned long i = 0; i < customers.size(); i++)
            restaurant.getTable(tableId)->getCustomers().push_back(customers.at(i)->clone());
        complete();
    }
}

// the function returns true if the table cannot be opened
bool OpenTable::isError(Restaurant& restaurant) {
    if(restaurant.getNumOfTables() <= tableId)
        return true;
    int capacity=restaurant.getTable(tableId)->getCapacity();
    unsigned long currentCusts=restaurant.getTable(tableId)->getCustomers().size();
    unsigned long newCusts=customers.size();
    bool p2 = capacity - currentCusts < newCusts;
    bool p3=restaurant.getTable(tableId)->isOpen();
    return p2 | p3;
}

void OpenTable::setInputStr(string args) { str=args; }

string OpenTable::toString() const{
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

OpenTable* OpenTable::clone() { return new OpenTable(*this); }

// ------------------------------------------- Action: Order
//constructor
Order::Order(int id):tableId(id),str("order") {}

void Order::act(Restaurant &restaurant) {
    if(restaurant.getNumOfTables()<=tableId || !restaurant.getTable(tableId)->isOpen())
        error("Table does not exist or is not open");
    else {
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
}
void Order::setInputStr(string args) { str=args; }

std::string Order::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}
    Order* Order::clone() { return new Order(*this); }

    Order::~Order()=default;
// ---------------------------- Action: Move Customer -----------------------------------------

//constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src),dstTable(dst),id(customerId),
                                                              str("move") {}

void MoveCustomer::act(Restaurant &restaurant) {
    if(isError(restaurant))
        error("Cannot move customer");
    else {
        Table *srcT=restaurant.getTable(srcTable);
        Table *dstT=restaurant.getTable(dstTable);
        Customer *cust=srcT->getCustomer(id);
        dstT->addCustomer(cust);
        srcT->removeCustomer(id);
        for (unsigned long i=0;i < srcT->getOrders().size();i++) {        // adding the customer's orders to the dst table
            if(srcT->getOrders().at(i).first==id)
                dstT->getOrders().push_back(srcT->getOrders().at(i));
        }
        srcT->getOrders()=removeOrders(srcT->getOrders(), id); // removing the customers orders from the src table
        if(srcT->getCustomers().empty())
            srcT->closeTable();
        complete();
        srcT=nullptr;
        dstT=nullptr;
    }
}
// the function returns a vector without the customer's orders
vector<OrderPair> MoveCustomer::removeOrders(vector<OrderPair> &orders, int id) {
    vector<OrderPair> afterRemoval;
    for(unsigned long i=0;i<orders.size();i++)
        if(orders.at(i).first!=id)
            afterRemoval.push_back(orders.at(i));
    return afterRemoval;
}

// the function returns true if the customer cannot be moved;
bool MoveCustomer::isError(Restaurant &restaurant) {
    bool p1=restaurant.getNumOfTables()<=max(srcTable, dstTable);
    bool p2=!restaurant.getTable(srcTable)->isOpen();
    bool p3=!restaurant.getTable(dstTable)->isOpen();
    bool p4=(unsigned long)restaurant.getTable(dstTable)->getCapacity() == restaurant.getTable(dstTable)->getCustomers().size();
    return p1 | p2 | p3 | p4;
}

void MoveCustomer::setInputStr(string args) { str=args; }

std::string MoveCustomer::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

MoveCustomer* MoveCustomer::clone() { return new MoveCustomer(*this); }

MoveCustomer::~MoveCustomer()=default;
// ---------------------------- Action: Close  -----------------------------------------

//Constructor
Close::Close(int id): str("close"), tableId(id) {}

void Close::act(Restaurant &restaurant) {
    if (restaurant.getNumOfTables() < tableId || !restaurant.getTable(tableId)->isOpen())
        error("Table does not exist or is not open");
    else {
        cout << "Table " << tableId << " was closed. Bill is " << restaurant.getTable(tableId)->getBill() << "NIS"
             << endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

void Close::setInputStr(string args) { str=args; }

std::string Close::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

Close* Close::clone(){ return new Close(*this); }

Close::~Close()=default;
// ---------------------------- Action: CloseAll  -----------------------------------------

CloseAll::CloseAll(): str("") {}

void CloseAll::act(Restaurant &restaurant) {
    BaseAction *close;
    for (int i=0; i<restaurant.getNumOfTables();i++)
        if(restaurant.getTable(i)->isOpen())
        {
            close=new Close(i);
            close->act(restaurant);
            delete close;
        }
    close=nullptr;
}

void CloseAll::setInputStr(string args) { str=args; }

std::string CloseAll::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}
CloseAll* CloseAll::clone() { return new CloseAll(*this); }

CloseAll::~CloseAll()=default;

// ------------------------------------------  PrintMenu  ------------------------------------------------------
PrintMenu::PrintMenu(): str("menu") {}

void PrintMenu::act(Restaurant &restaurant) {
    for(auto i=restaurant.getMenu().begin(); i!=restaurant.getMenu().end();i++) {
        cout << i->toString();
        cout << "NIS" << endl;
    }
    complete();
}

void PrintMenu::setInputStr(string args) { str=args; }

string PrintMenu::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}
PrintMenu* PrintMenu::clone() { return new PrintMenu(*this);}

PrintMenu::~PrintMenu()=default;
// ------------------------------------------  PrintTableStatus  ------------------------------------------------------
PrintTableStatus::PrintTableStatus(int id):tableId(id), str("status") {}

void PrintTableStatus::act(Restaurant &restaurant) {
    cout << "Table " << tableId << " Status: ";
    Table *t=restaurant.getTable(tableId);
    if(t->isOpen()) {
        cout << "open" << endl;
        cout << "Customers:" << endl;
        for (auto i = t->getCustomers().begin(); i != t->getCustomers().end(); i++)
            cout << (*i)->getId() << " " << (*i)->getName() << endl;
        cout << "Orders:" << endl;
        for (auto i = t->getOrders().begin(); i != t->getOrders().end(); i++)
            cout << i->second.getName() << " " << i->second.getPrice() << "NIS " << i->first << endl;
        cout << "Current Bill: " << t->getBill() << "NIS" << endl;
    }
    else
        cout << "closed" << endl;
    complete();
    t=nullptr;
}

void PrintTableStatus::setInputStr(string args) { str=args; }

std::string PrintTableStatus::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

PrintTableStatus* PrintTableStatus::clone() { return new PrintTableStatus(*this); }

PrintTableStatus::~PrintTableStatus()=default;
// ---------------------------- Action: PrintActionsLog  -----------------------------------------

//Constructor
PrintActionsLog::PrintActionsLog():str("log") {}

void PrintActionsLog::act(Restaurant &restaurant){
    for(unsigned long i=0;i<restaurant.getActionsLog().size();i++) {
        cout << restaurant.getActionsLog().at(i)->toString() << endl;
    }
    complete();
}

void PrintActionsLog::setInputStr(string args) { str=args; }

std::string PrintActionsLog::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

PrintActionsLog* PrintActionsLog::clone() { return new PrintActionsLog(*this); }

PrintActionsLog::~PrintActionsLog()=default;
// ------------------------------------------  BackupRestaurant  ------------------------------------------------------
BackupRestaurant::BackupRestaurant(): str("backup") {}

void BackupRestaurant::act(Restaurant &restaurant) {
    delete backup;
    backup=new Restaurant(restaurant);
    complete();
}

void BackupRestaurant::setInputStr(string args) { str=args; }

std::string BackupRestaurant::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

BackupRestaurant* BackupRestaurant::clone() { return new BackupRestaurant(*this); }

BackupRestaurant::~BackupRestaurant()=default;
// ------------------------------------------  RestoreRestaurant  ------------------------------------------------------
RestoreResturant::RestoreResturant(): str("restore") {}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup==nullptr)
        error("No backup available");
    else {
        restaurant = *backup;
        complete();
    }
}

void RestoreResturant::setInputStr(string args) { str=args; }

std::string RestoreResturant::toString() const {
    string s=str+" "+convertToString(getStatus());
    if(getStatus()==ERROR)
        s+=": "+getErrorMsg();
    return s;
}

RestoreResturant* RestoreResturant::clone() { return new RestoreResturant(*this); }

RestoreResturant::~RestoreResturant()=default;