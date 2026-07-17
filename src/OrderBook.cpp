#include <iostream>
#include <vector>
#include <sstream>
#include "order.hpp"



void OrderBook::processOrder(const std::string& line)
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
    else if (completedOrder.command == Command::remove)
    {
        int orderId = completedOrder.orderData.orderId;

        if (completedOrder.side == Side::buy)
        {
            for(int i = 0; i< bids.size(); i++)
            {
                if(bids[i].orderId == orderId)
                {
                    bids.erase(bids.begin() + i);
                    break;
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
                    break;
                }
            } 

        }

    }
}

// Make orderId the input and then print function understands what to ouput 

void OrderBook::printOrder(const int orderId)
{
    for(int i = 0; i < bids.size(); i++)
    {
        if(bids[i].orderId = orderId)
        {
            std::cout << "You ordered: " << "\n" <<
            bids[i].orderId << " " << bids[i].quantity << " " << bids[i].price << "\n";
            break;
        }
    }
    for(int i = 0; i < asks.size(); i++)
    {
        if(asks[i].orderId = orderId)
        {
            std::cout << "You want to sell: " << "\n" <<
            asks[i].orderId << " " << asks[i].quantity << " " << asks[i].price << "\n";
            break;
        }
    }
}

void OrderBook::printAll(std::string& type)
{
    if(type == "BIDS")
    {
        for(int i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
    }
    else if (type == "ASKS")
    {
        for(int i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }  
    }
    else if (type == "ALL")
    {
        for(int i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
        for(int i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }  
    }
}
      

ParsedInput OrderBook::orderCreate(const std::string& line)
{
    std::istringstream iss(line);
    std::string command;
    int id;
    std::string side;
    int price;
    int quantity;
    Order orderInput;
    ParsedInput completedOrder;

    iss >> command;

    if (command == "ADD")
    {
        iss >> command >> id >> side >> price >> quantity;
        orderInput.orderId = id;
        orderInput.price = price;
        orderInput.quantity = quantity;

        Command orderType = Command::add;
        completedOrder.command = orderType;
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
    }

    else if(command == "REMOVE"){
        iss >> command >> id;
        orderInput.orderId = id;

        Command orderType = Command::remove;
        completedOrder.command = orderType;
    }

    completedOrder.orderData = orderInput;
    return completedOrder;
}

void OrderBook::orderOrder()
{
    while(true)
    {
    std::cout << "Please input your new order: " << "\n";
    std::string newOrder = getLine();
    processOrder(newOrder);
    if(newOrder.empty())
    {
        false;
    }
    }
}

std::string OrderBook::getLine()
    {
        std::string line;
        std::getline(std::cin, line);
        return line;
    };
