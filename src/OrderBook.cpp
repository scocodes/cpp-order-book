#include <iostream>
#include <vector>
#include <sstream>
#include "order.hpp"



ParsedInput orderCreate(std::string line)
{

    std::istringstream iss(line);
    std::string command;
    int id;
    std::string side;
    int price;
    int quantity;

    iss >> command >> side >> price >> quantity;
    

    Order orderInput;
    orderInput.orderId = id;
    orderInput.price = price;
    orderInput.quantity = quantity;

    ParsedInput completedOrder;

    if (command == "ADD")
    {
        Command orderType = Command::add;
        completedOrder.command = orderType;
    }

    else{
        Command orderType = Command::remove;
        completedOrder.command = orderType;
    }

    if (side == "BUY")
    {
        Side side = Side::buy;
        completedOrder.side = side;
    }
    else
    {
        Side side = Side::sell;
        completedOrder.side = side;
    }
  
    completedOrder.orderData = orderInput;
    
    return completedOrder;
};

class OrderBook
{
    public:
    private:
        std::vector<Order> bids;
        std::vector<Order> asks;

        void addOrder(ParsedInput completedOrder){
            if (completedOrder.command == Command::add){
                if (completedOrder.side == Side::buy){
                    bids.push_back(completedOrder.orderData);
                }
                else if (completedOrder.side == Side::sell){
                    asks.push_back(completedOrder.orderData);
                };
            };  
        };

        void removeOrder(ParsedInput completedOrder)
        {
            if (completedOrder.command == Command::remove)
            {
                int orderId = completedOrder.orderData.orderId;

                if (completedOrder.side == Side::buy)
                {
                    for(int i = 0; i< bids.size(); i++)
                    {
                        bids.begin() + i;
                        if(bids[i].orderId == orderId)
                        {
                            bids.erase(bids.begin() + i);
                            break;
                        };
                    };
                };
                if (completedOrder.side == Side::sell)
                {
                    for(int i = 0; i< asks.size(); i++)
                    {
                        asks.begin() + i;
                        if(asks[i].orderId == orderId)
                        {
                            asks.erase(asks.begin() + i);
                            break;
                        };
                    };    

                };

            
            };
           
        };    

};  

