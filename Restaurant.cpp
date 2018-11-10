//
// Created by simo on 11/9/18.
//

#include "Restaurant.h"
using namespace std;

Restaurant::Restaurant(const std::string &configFilePath) {
    int index=0;
    int numOfTables=readNumOfTables(index, configFilePath);
    createTables(index, configFilePath, numOfTables);
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
        string numOfPlaces="";
        while(file.at(i)!=',')
            numOfPlaces+=file.at(i);
        int numberOfPlaces=stoi(numOfPlaces);
        tables.push_back(new Table(numberOfPlaces));
        i++;
    }
    i++;
}