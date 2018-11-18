#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include "Customer.h"
#include "Table.h"
#include <string>
using namespace std;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void setInputStr(string args)=0;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string convertToString(ActionStatus stat) const;
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);      // Constructor
    OpenTable(const OpenTable &other);                              // Copy Constructor
    OpenTable(OpenTable &&other);                                   // Move Constructor
    OpenTable& operator=(const OpenTable &other);                   // Copy Operator=
    OpenTable& operator=(OpenTable &&other);                        // Move Operator=
    virtual ~OpenTable();                                           // Destructor
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    bool isError(Restaurant &restaurant);
    void clear();
    const int tableId;
    string str;
    std::vector<Customer *> customers;
};

class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual void setInputStr(string args);
private:
    const int tableId;
    string str;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
    const int srcTable;
    const int dstTable;
    const int id;
    bool isError(Restaurant &restaurant);
    vector<OrderPair> removeOrders(vector<OrderPair> &orders, int id);
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
    const int tableId;

};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual void setInputStr(string args);
private:
    string str;
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    const int tableId;
    string str;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual void setInputStr(string args);
private:
    string str;
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
};


#endif