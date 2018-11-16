#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Restaurant.h"
#include "Table.h"

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
    std::string convertToString(ActionStatus stat);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    OpenTable(OpenTable &other);
    OpenTable& operator=(const OpenTable &other);
    OpenTable& operator=(OpenTable &&other);
    virtual ~OpenTable();
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
    Order(Order &other);
    Order& operator=(const Order &other);
    Order& operator=(Order &&other);
    virtual ~Order();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual void setInputStr(string args);
private:
    const int tableId;
    string str;
    void clear();
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer &other);
    MoveCustomer& operator=(const MoveCustomer &other);
    MoveCustomer& operator=(MoveCustomer &&other);
    virtual ~MoveCustomer();
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
    const int srcTable;
    const int dstTable;
    const int id;
    void clear();
    bool isError(Restaurant &restaurant);
    vector<OrderPair> removeOrders(vector<OrderPair> &orders, int id);
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close &other);
    Close& operator=(const Close &other);
    Close& operator=(Close &&other);
    virtual ~Close();
    void act(Restaurant &restaurant);
    virtual void setInputStr(string args);
    std::string toString() const;
private:
    string str;
    const int tableId;
    void clear();
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
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