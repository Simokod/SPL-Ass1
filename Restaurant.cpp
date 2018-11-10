//
// Created by simo on 11/9/18.
//
#include "Restaurant.h"
using namespace std;

//Restaurant constructor
Restaurant::Restaurant(const std::string &configFilePath) {
    int index=0;
    numOfTables=readNumOfTables(index, configFilePath);
    createTables(index, configFilePath, numOfTables);
    createMenu(index, configFilePath);
}
//Restaurant copy constructor
Restaurant::Restaurant(Restaurant &other)
{
    open=other.open;
    numOfTables=other.numOfTables;
    menu=other.menu;
    tables=other.tables;

}
vector<Dish>& getMenu(){
    return menu;
}
int getNumOfTables() const{
    return numOfTables;
}

int Restaurant::readNumOfTables(int &i, const string &file){
    // skipping first line
    while(file.at(i)!='\n')
        i++;
    i++;
    // getting number of tables from 2nd line
    string numOfTables="";
    while(file.at(i)!='\n') {
        numOfTables+=file.at(i);
        i++;
    }
    i++;    // skipping '\n' char
    return stoi(numOfTables);
}

void Restaurant::createTables(int &i, const std::string &file, int numOfTables) {
    // skipping line of comment
    while(file.at(i)!='\n')
        i++;
    i++;    // going down a line
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
}
void Restaurant::createMenu(int &i, const std::string &file){
    // skipping line of comment
    while(file.at(i)!='\n')
        i++;
    i++;    // going down a line
    int id=0;
    while(file.at(i)!=null) {    // creating new menus and pushing them into the Menu vector
        string dishName="";
        while(file.at(i)!=','){
            dishName+=file.at(i);
            i++
        }
        i++;
        string type="";
        while(file.at(i)!=','){
            type+=file.at(i);
            i++;
        }
        dishType dType=convert(type);
        string price="";
        while(file.at(i)!=','){
            price+=file.at(i);
            i++;
        }
        int dPrice=stoi(price);
        menu.push_back(new Dish(id,dishName,dPrice,dType));
        id++;
        i++;
    }
}


dishType Restaurant::convert(string str){
    if (str=="VEG")
        return VEG;
    if (str=="SPC")
        return SPC;
    if (str=="BVG")
        return BVG;
    if (str=="ALC")
        return ALC;
    return;
}