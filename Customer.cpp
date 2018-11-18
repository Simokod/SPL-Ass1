//
// Created by barsimo on 11/6/18.
//

#include "Customer.h"
using namespace std;

    Customer::Customer(string c_name, int c_id): name(c_name), id(c_id){}

    int Customer::getId() const {
        const int &_id=id;
        return _id;
    }

    string Customer::getName() const {
        const string &_name=name;
        return _name;
    }

//----------------------------------------------Vegetarian Customer--------------------------------------
    VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name, id) {}

    vector<int> VegetarianCustomer::order(const vector<Dish> &menu) {
        vector<int> orders;
        // adding the first vegeterian dish to the order
        for (auto i = menu.begin(); i != menu.end(); ++i)
            if(i->getType()==VEG) {
                orders.push_back(i->getId());
                break;
            }
        if(orders.size()==0) return orders; // no vegeterian dish - no order at all
        // adding the most expensive beverage dish to the order
        int expensive=0;
        for (auto i = menu.begin(); i != menu.end(); ++i)
            if(i->getType()==BVG)
                if(i->getPrice()>expensive){
                    orders.push_back(i->getId());
                    break;
                }
        return orders;
    }

    std::string VegetarianCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Vegeterian Customer";
        return s1+s2+s3;
    }

    VegetarianCustomer* VegetarianCustomer::clone() { return new VegetarianCustomer(*this); }
//----------------------------------------------Cheap Customer--------------------------------------
    CheapCustomer::CheapCustomer(string name, int id): Customer(name, id), ordered(false) {}

    vector<int> CheapCustomer::order(const vector<Dish> &menu) {
        vector<int> orders;
        if(!ordered)
        {
            auto i = menu.begin();
            int cheapest=i->getPrice();
            int cheapestId=0;
            for( i ;i!=menu.end();i++)
                if(i->getPrice()<cheapest)
                {
                    cheapest=i->getPrice();
                    cheapestId=i->getId();
                }
            orders.push_back(cheapestId);
            ordered = true;
        }
        return orders;
    }

    std::string CheapCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Cheap Customer";
        return s1+s2+s3;
    }

    CheapCustomer* CheapCustomer::clone() { return new CheapCustomer(*this); }
//----------------------------------------------Spicy Customer--------------------------------------
    SpicyCustomer::SpicyCustomer(string name, int id): Customer(name, id), ordered(false) {}

    vector<int> SpicyCustomer::order(const vector<Dish> &menu) {
        vector<int> orders;
        if(!ordered) {  // first order: searching for the most expensive spicy dish
            ordered=true;
            auto i = menu.begin();
            int expensivePrice=0;
            int expensiveId=-1;
            for (i ; i != menu.end(); i++)
                if (i->getType() == SPC)
                    if(i->getPrice()>expensivePrice)
                    {
                        expensivePrice=i->getPrice();
                        expensiveId=i->getId();
                    }
            if(expensiveId!=-1)         // no spicy dish was found
            {
                orders.push_back(expensiveId);
                ordered=false;
            }
        }
        else    // after the first order: searching for the cheapest beverage
        {
            // finding the first BVG Dish in order to do the comparisons
            int minPrice;
            int minPriceId=-1;
            auto i=menu.begin();
            for( i ; i != menu.end(); i++)
                if (i->getType() == BVG) {
                    minPrice = i->getPrice();
                    minPriceId=i->getId();
                    break;
                }
            // searching for a cheaper BVG Dish
            for ( i ; i != menu.end(); i++)
                if(i->getType() == BVG)
                    if(i->getPrice()<minPrice) {
                        minPrice=i->getPrice();
                        minPriceId=i->getId();
                }
            if(minPriceId!=-1)
                orders.push_back(minPriceId);
        }
        return orders;
    }

    string SpicyCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Spicy Customer";
        return s1+s2+s3;
    }

    SpicyCustomer* SpicyCustomer::clone() { return new SpicyCustomer(*this); }
//----------------------------------------------Alchoholic Customer--------------------------------------
    AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name, id), lastOrder(-1) {}

   std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
       vector<int> orders;
       int lastOrderPrice;
       int minPrice;
       int minPriceId;
       if (lastOrder == -1) {
           lastOrderPrice = 0;
       } else {
           lastOrderPrice = menu.at(lastOrder).getPrice();
       }
       bool ordered=false;
       // finding the first ALC Dish in order to do the comparisons
       auto i=menu.begin();
       for( i ; i != menu.end(); i++)
           if (i->getType() == ALC && i->getPrice() > lastOrderPrice) {
               minPrice = i->getPrice();
               minPriceId=i->getId();
               ordered=true;
               break;
           }
       if(!ordered)
           return orders;
       // searching for a cheaper ALC Dish (more expensive than last order)
       for( i ; i != menu.end(); i++)
           if (i->getType() == ALC && i->getPrice()>lastOrderPrice & i->getPrice() < minPrice)
           {
               minPrice=i->getPrice();
               minPriceId=i->getId();
           }
       lastOrder=minPriceId;
       if(lastOrder!=-1)
           orders.push_back(lastOrder);
       return orders;
   }

    string AlchoholicCustomer::toString() const {
        string s1=getName();
        string s2=", "+getId();
        string s3=", Alchoholic Customer";
        return s1+s2+s3;
    }

    AlchoholicCustomer* AlchoholicCustomer::clone() { return new AlchoholicCustomer(*this); }