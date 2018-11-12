#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const Restaurant &other);
    Restaurant(Restaurant &&other);
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    Restaurant& operator=(const Restaurant &other);
    Restaurant& operator=(Restaurant &&other);
    virtual ~Restaurant();


private:
    bool open;
    int numOfTables;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int readNumOfTables(int &index, const std::string &file);

    void createTables(int &index, const std::string &file, int numOfTables);
    void createMenu(int &i, const std::string &file);
    DishType Restaurant::convert(std::string str);
    void clear();

};

#endif