//
// Created by simo on 11/9/18.
//
#include "Restaurant.h"
#include "Dish.h"
using namespace std;

extern Restaurant* backup;
//Restaurant constructor
Restaurant::Restaurant(const std::string &configFilePath): customersId(0) {
    int index=0;
    numOfTables=readNumOfTables(index, configFilePath);
    createTables(index, configFilePath, numOfTables);
    createMenu(index, configFilePath);
}
//Restaurant copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open), numOfTables(other.numOfTables), menu(other.menu),
                                                 customersId(other.customersId), tables(other.tables) {}
//Restaurant move constructor
Restaurant::Restaurant(Restaurant &&other): open(other.open), numOfTables(other.numOfTables), menu(other.menu),
                                            customersId(other.customersId), tables(other.tables)
{
    other.menu.clear();
    other.menu.shrink_to_fit();
    other.tables.clear();
    other.tables.shrink_to_fit();
    other.open = false;
    other.numOfTables=0;
    other.customersId=0;
}

// Restaurant copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant &other){
    if(this!=&other) {
        clear();
        numOfTables=other.numOfTables;
        open=other.open;
        tables=other.tables;
        menu=other.menu;
        customersId=other.customersId;
    }
    return *this;
}
// Restaurant move assignment operator
Restaurant& Restaurant::operator=(Restaurant &&other){
    if(this!=&other) {
        clear();                    // assign other into thi
        numOfTables=other.numOfTables;
        open = other.open;
        tables=other.tables;
        menu=other.menu;
        numOfTables=other.numOfTables;
        customersId=other.customersId;
        // delete other
        other.menu.clear();
        other.menu.shrink_to_fit();
        other.tables.clear();
        other.tables.shrink_to_fit();
        other.open = false;
        other.numOfTables=0;
        other.customersId=0;
    }
    return *this;
}

// Restaurant destructor
Restaurant::~Restaurant() { clear(); }

//Getters
std::vector<Dish>& Restaurant::getMenu() { return menu; }
std::vector<Table>& Restaurant::getTables()  {return tables;}
int Restaurant::getNumOfTables() const  { return numOfTables; }
const std::vector<BaseAction*>& Restaurant::getActionsLog() const { return actionsLog; }

// opening the restaurant to the world!
void Restaurant::start() {
    cout << "Restaurant is now open!" << endl;
    string s;
    cin >> s;
    BaseAction *action;
    while(s.substr(0,8)!="closeall") {
        Actions input=convertAct(s);
        switch (input)
        {
            case OPEN:
                action=actionOpenTable(s);
                action->act(*this);
                break;
            case ORDER:
                action=actionOrder(s);
                action->act(*this);
                break;
            case MOVE:
                action=actionMove(s);
                action->act(*this);
                break;
            case CLOSE:
                action=actionClose(s);
                action->act(*this);
                break;
            case MENU:
                action=actionPrintMenu(s);
                action->act(*this);
                break;
            case STATUS:
                action=actionPrintTableStatus(s);
                action->act(*this);
                break;
            case LOG:
                action=actionPrintActionsLog(s);
                action->act(*this);
                break;
            case BACKUP:
                action=actionBackupRestaurant(s);
                action->act(*this);
                break;
            case RESTORE:
                action=actionRestoreRestaurant(s);
                action->act(*this);
                break;
        }
        actionsLog.push_back(action);
        cin >> s;
    }
    action=actionCloseAll();
    action->act(*this);
}
// returns an OpenTable BaseAction
OpenTable* Restaurant::actionOpenTable(const std::string s) {
    int i=5;
    string tableIdStr;
    while(s.at(i)!=' ')             // reading table ID
        tableIdStr+=s.at(i);
    int tableId=stoi(tableIdStr);
    string name, type;
    vector<Customer*> customers;
    for( i ; i<s.size();i++){
        while(s.at(i)!=','){        // reading costumer name
            name+=s.at(i);
            i++;
        }
        type=s.substr(i+1,3);       // reading costumer type
        i=i+5;                      // moving index to next costumer
        // creating the customer
        Customer *cus;
        if(type=="veg")
            cus=new VegetarianCustomer(name, customersId);
        else
        if(type=="chp")
            cus=new CheapCustomer(name, customersId);
        else
        if(type=="spc")
            cus=new SpicyCustomer(name, customersId);
        else
            cus=new AlchoholicCustomer(name, customersId);
        customersId++;
        customers.push_back(cus);
    }
    OpenTable *action=new OpenTable(tableId, customers);
    action->setInputStr(s);
    return action;
}
// returns an Order BaseAction
Order* Restaurant::actionOrder(std::string s) {
    string tableIdStr;
    for(int i=6;i<s.size();i++)
        tableIdStr+=s.at(i);
    int tableId=stoi(tableIdStr);
    Order *action=new Order(tableId);
    action->setInputStr(s);
    return action;
}
// returns a Move BaseAction
MoveCustomer* Restaurant::actionMove(std::string s) {
    string srcStr, dstStr, customerIdStr;
    int i=5;
    while(s.at(i)!=' '){            // reading source table id
        srcStr+=s.at(i);
        i++;
    }
    i++;
    while(s.at(i)!=' '){            // reading destination table id
        dstStr+=s.at(i);
        i++;
    }
    i++;
    while(s.at(i)!=' '){            // reading customer id
        customerIdStr+=s.at(i);
        i++;
    }
    int src=stoi(srcStr);
    int dst=stoi(dstStr);
    int customerId=stoi(customerIdStr);
    MoveCustomer *action=new MoveCustomer(src, dst, customerId);
    action->setInputStr(s);
    return action;
}
// returns a Close BaseAction
Close* Restaurant::actionClose(std::string s) {
    string tableIdStr;
    for(int i=6;i<s.size();i++)
        tableIdStr+=s.at(i);
    int tableId=stoi(tableIdStr);
    Close *action=new Close(tableId);
    action->setInputStr(s);
    return action;
}
// returns a CloseAll BaseAction
CloseAll* Restaurant::actionCloseAll() {
    return new CloseAll();
}
// returns a PrintMenu BaseAction
PrintMenu* Restaurant::actionPrintMenu(std::string s) {
    PrintMenu *action=new PrintMenu();
    action->setInputStr(s);
    return action;
}
// returns a PrintTableStatus BaseAction
PrintTableStatus* Restaurant::actionPrintTableStatus(std::string s) {
    string tableIdStr;
    for(int i=7;i<s.size();i++)
        tableIdStr+=s.at(i);
    int tableId=stoi(tableIdStr);
    PrintTableStatus *action=new PrintTableStatus(tableId);
    action->setInputStr(s);
    return action;
}
// returns a PrintActionsLog BaseAction
PrintActionsLog* Restaurant::actionPrintActionsLog(std::string s) {
    PrintActionsLog *action=new PrintActionsLog();
    action->setInputStr(s);
    return action;
}
// returns a BackUpRestaurant BaseAction
BackupRestaurant* Restaurant::actionBackupRestaurant(std::string s) {
    BackupRestaurant *action=new BackupRestaurant();
    action->setInputStr(s);
    return action;
}
// returns a RestoreRestaurant BaseAction
RestoreResturant* Restaurant::actionRestoreRestaurant(std::string s) {
    RestoreResturant *action=new RestoreResturant();
    action->setInputStr(s);
    return action;
}
//Assistant functions
void Restaurant::clear() {
    menu.clear();
    menu.shrink_to_fit();
    for(int i=0;i<tables.size();i++)
    {
        delete tables.at(i);
        tables.at(i) = nullptr;
    }
    tables.shrink_to_fit();
    numOfTables=0;
    customersId=0;
    open=false;
}
// the function reads the config file and returns the number of tables in the restaurant
int Restaurant::readNumOfTables(int &i, const string &file){
    // skipping first line
    if(file.at(i)=='#')
        while(file.at(i)!='\n')
            i++;
    while(i=='\n') i++;     // going down empty lines
    // getting number of tables from 2nd line
    string numOfTables=
    while(file.at(i)!='\n') {
        numOfTables+=file.at(i);
        i++;
    }
    while(i=='\n') i++;    // going down empty lines
    return stoi(numOfTables);
}
// filling the tables vector
void Restaurant::createTables(int &i, const std::string &file, int numOfTables) {
    // skipping line of comment
    if(file.at(i)=='#')
        while(file.at(i)!='\n')
            i++;
    while(i=='\n') i++;    // going down empty lines

    for(int j=0;j<numOfTables;j++) {    // creating new tables and pushing them into the Tables vector
        string numOfPlaces;
        while (file.at(i) != ','){
            numOfPlaces += file.at(i);
            i++;
        }
        int numberOfPlaces=stoi(numOfPlaces);
        tables.push_back(new Table(numberOfPlaces));
        i++;
    }
    i++;
    while(i=='\n') i++;    // going down empty lines
}
// filling the menu vector
void Restaurant::createMenu(int &i, const std::string &file){
    // skipping line of comment
    if(file.at(i)=='#')
        while(file.at(i)!='\n')
            i++;
    while(i=='\n') i++;    // going down empty lines

    int id=0;
    for( i ; i<file.size()-i;i++) {    // creating new menus and pushing them into the Menu vector
        string dishName;
        while(file.at(i)!=','){
            dishName+=file.at(i);
            i++;
        }
        i++;
        string type;
        while(file.at(i)!=','){
            type+=file.at(i);
            i++;
        }
        DishType dType=convertDish(type);
        string price;
        while(file.at(i)!=','){
            price+=file.at(i);
            i++;
        }
        int dPrice=stoi(price);
        menu.push_back(*new Dish(id,dishName,dPrice,dType));
        id++;
        i++;
    }
}


// the function converts a given string to the corresponding dish type
DishType Restaurant::convertDish(std::string str) {
    if (str=="VEG")
        return VEG;
    if (str=="SPC")
        return SPC;
    if (str=="BVG")
        return BVG;
    if (str=="ALC")
        return ALC;
}
// converting input to an action
Actions Restaurant::convertAct(std::string str) {
    string actionStr;
    int i = 0;
    while (str.at(i) != ' ')
        actionStr += str.at(i);
    if (actionStr == "open")
        return OPEN;
    if (actionStr == "order")
        return ORDER;
    if (actionStr == "move")
        return MOVE;
    if (actionStr == "close")
        return CLOSE;
    if (actionStr == "closeall")
        return CLOSEALL;
    if (actionStr == "menu")
        return MENU;
    if (actionStr == "status")
        return STATUS;
    if (actionStr == "log")
        return LOG;
    if (actionStr == "backup")
        return BACKUP;
    if (actionStr == "restore")
        return RESTORE;
    return WRONG;
}