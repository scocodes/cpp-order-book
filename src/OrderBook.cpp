#include <iostream>
#include <vector>
#include <sstream>
#include "order.hpp"



class OrderBook
{
    public:

        void addOrder(const std::string& line)
        {
            ParsedInput completedOrder = orderCreate(line);
            if (completedOrder.command == Command::add)
            {
                if (completedOrder.side == Side::buy)
                {
                    bids.push_back(completedOrder.orderData);
                }
                else if (completedOrder.side == Side::sell)
                {
                    asks.push_back(completedOrder.orderData);
                }
            }
        }

        bool removeOrder(const std::string& line)
        {
            ParsedInput completedOrder = orderCreate(line);
            if (completedOrder.command == Command::remove)
            {
                int orderId = completedOrder.orderData.orderId;

                if (completedOrder.side == Side::buy)
                {
                    for(int i = 0; i< bids.size(); i++)
                    {
                        if(bids[i].orderId == orderId)
                        {
                            bids.erase(bids.begin() + i);
                            return true;
                        }
                    }
                    }
                
                if (completedOrder.side == Side::sell)
                {
                    for(int i = 0; i< asks.size(); i++)
                    {
                        if(asks[i].orderId == orderId)
                        {
                            asks.erase(asks.begin() + i);
                            return true;
                        }
                    } 

                }

            }
            return false;
        }
        void print(std::string line)
        {
            if (line == "BIDS")
            {
                for(int i = 0; i < bids.size(); i++)
                {
                    std::cout << bids[i].orderId << " " << bids[i].quantity << " " << bids[i].price << "\n";
                }

            }
            else if (line == "ASKS")
            {
                for(int i = 0; i < asks.size(); i++)
                {
                    std::cout << asks[i].orderId << " " << asks[i].quantity << " " << asks[i].price << "\n";
                }

            }
        }
              
    private:
        std::vector<Order> bids;
        std::vector<Order> asks;

        ParsedInput orderCreate(const std::string& line)
        {
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

            if (command == "ADD")
            {
                Command orderType = Command::add;
                completedOrder.command = orderType;
            }

            else if(command == "REMOVE"){
                Command orderType = Command::remove;
                completedOrder.command = orderType;
            }

            if (side == "BUY")
            {
                Side side = Side::buy;
                completedOrder.side = side;
            }
            else if (side == "SELL")
            {
                Side side = Side::sell;
                completedOrder.side = side;
            }
        
            completedOrder.orderData = orderInput;
            
            return completedOrder;
        }
};  

