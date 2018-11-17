#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <bits/stdc++.h>


extern Restaurant* backup;

enum Actions{OPEN, ORDER, MOVE, CLOSE, CLOSEALL, MENU, STATUS, LOG, BACKUP, RESTORE, WRONG};

class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    Table* getTable(int ind);
    std::vector<Dish>& getMenu();

    virtual ~Restaurant();                           // destructor
    Restaurant(const Restaurant &other);             // copy constructor
    Restaurant(Restaurant &&other);                  // move constructor
    Restaurant& operator=(const Restaurant &other);  // copy operator=
    Restaurant& operator=(Restaurant &&other);       // move operator=

private:
    bool open;
    int numOfTables;
    int customersId;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;

    int readNumOfTables(int &index, const std::string &file);
    void createTables(int &index, const std::string &file, int numOfTables);
    void createMenu(int &i, const std::string &file);
    void clear();
    DishType convertDish(std::string str);
    Actions convertAct(std::string str);
    std::string fileToString(fstream &file);

    OpenTable* actionOpenTable(std::string s);
    Order* actionOrder(std::string s);
    MoveCustomer* actionMove(std::string s);
    Close* actionClose(std::string s);
    CloseAll* actionCloseAll();
    PrintMenu* actionPrintMenu(std::string s);
    PrintTableStatus* actionPrintTableStatus(std::string s);
    PrintActionsLog* actionPrintActionsLog(std::string s);
    BackupRestaurant* actionBackupRestaurant(std::string s);
    RestoreResturant* actionRestoreRestaurant(std::string s);
};

#endif