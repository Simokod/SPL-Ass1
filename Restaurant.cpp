//
// Created by simo on 11/9/18.
//
#include "Restaurant.h"
#include "Dish.h"
using namespace std;

//Restaurant constructor
Restaurant::Restaurant(const std::string &configFilePath) {
    int index=0;
    numOfTables=readNumOfTables(index, configFilePath);
    createTables(index, configFilePath, numOfTables);
    createMenu(index, configFilePath);
}
//Restaurant copy constructor
Restaurant::Restaurant(const Restaurant &other): open(other.open), numOfTables(other.numOfTables), menu(other.menu),
                                                             tables(other.tables) {}
//Restaurant move constructor
Restaurant::Restaurant(Restaurant &&other): open(other.open), numOfTables(other.numOfTables), menu(other.menu),
                                            tables(other.tables)
{
    other.menu.clear();
    other.menu.shrink_to_fit();
    other.tables.clear();
    other.tables.shrink_to_fit();
    other.open = false;
    other.numOfTables=0;
}

// Restaurant copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant &other){
    if(this!=&other) {
        clear();
        numOfTables=other.numOfTables;
        open=other.open;
        tables=other.tables;
        menu=other.menu;
    }
    return *this;
}
// Restaurant move assignment operator
Restaurant& Restaurant::operator=(Restaurant &&other){
    if(this!=&other) {
        clear();
        numOfTables=other.numOfTables;
        open = other.open;
        tables=other.tables;
        menu=other.menu;
        numOfTables=other.numOfTables;

        other.menu.clear();
        other.menu.shrink_to_fit();
        other.tables.clear();
        other.tables.shrink_to_fit();
        other.open = false;
        other.numOfTables=0;
    }
    return *this;
}

// Restaurant destructor
Restaurant::~Restaurant() { clear(); }


//Getters
std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}

int Restaurant::getNumOfTables() const  {
    return numOfTables;
}
//------------------------------------------------------------- NOT FINISHED--------------------------------
// opening the restaurant to the world!
void Restaurant::start() {
    cout << "Restaurant is now open!" << endl;
    string s;
    cin >> s;
    while(s!="closeall")
    {
        Actions input=convertAct(s);
    }

}
// converting input from the client to an action
Actions Restaurant::convertAct(std::string str) {
    string actionStr="";
    int i=0;
    while(str.at(i)!=' ')
        actionStr+=str.at(i);
    if(actionStr=="open")
        return OPEN;
    if(actionStr=="order")
        return ORDER;
    if(actionStr=="move")
        return MOVE;
    if(actionStr=="close")
        return CLOSE;
    if(actionStr=="closeall")
        return CLOSEALL;
    if(actionStr=="menu")
        return MENU;
    if(actionStr=="status")
        return STATUS;
    if(actionStr=="log")
        return LOG;
    if(actionStr=="backup")
        return BACKUP;
    if(actionStr=="restore")
        return RESTORE;

    return WRONG;
}


//Assistant functions
void Restaurant::clear(){
    menu.clear();
    menu.shrink_to_fit();
    for(int i=0;i<tables.size();i++)
    {
        delete tables.at(i);
        tables.at(i) = nullptr;
    }
    tables.shrink_to_fit();
    numOfTables=0;
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
    string numOfTables="";
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
        string numOfPlaces = "";
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
        string dishName="";
        while(file.at(i)!=','){
            dishName+=file.at(i);
            i++;
        }
        i++;
        string type="";
        while(file.at(i)!=','){
            type+=file.at(i);
            i++;
        }
        DishType dType=convert(type);
        string price="";
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
DishType Restaurant::convert(std::string str) {
    if (str=="VEG")
        return VEG;
    if (str=="SPC")
        return SPC;
    if (str=="BVG")
        return BVG;
    if (str=="ALC")
        return ALC;
}