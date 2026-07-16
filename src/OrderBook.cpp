#include <iostream>
#include <vector>
#include <sstream>
#include "order.hpp"



ParsedInput orderCreate(std::string line){

    std::istringstream iss(line);
    std::string command;
    int id;
    std::string side;
    int price;
    int quantity;

    iss >> command >> id >> side >> price >> quantity;
    

    Order orderInput;
    orderInput.orderId = id;
    orderInput.price = price;
    orderInput.quantity = quantity;

    ParsedInput completedOrder;

    if (command == "ADD"){
        Command orderType = Command::add;
        completedOrder.command = orderType;
    }

    else{
        Command orderType = Command::remove;
        completedOrder.command = orderType;
    }

    if (side == "BUY"){
        Side side = Side::buy;
        orderInput.side = side;
    }
    else{
        Side side = Side::sell;
        orderInput.side = side;
    }
  
    completedOrder.orderData = orderInput;
    
    return completedOrder;
};

class OrderBook{
    public:
    private:
        void addOrder(ParsedInput completedOrder){
            



        }
};
