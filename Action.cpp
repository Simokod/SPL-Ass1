//
// Created by simo on 11/9/18.
//

#include "Action.h"

using namespace std;

ActionStatus BaseAction::getStatus() const { return status; }

void BaseAction::complete() { status = COMPLETED; }

std::string BaseAction::getErrorMsg() const { return errorMsg; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    errorMsg=errorMsg;
}

// ---------------------------- Action: Open Table -----------------------------------------
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id) {}
void OpenTable::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId).)
}