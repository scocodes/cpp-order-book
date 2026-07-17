#pragma once
#include <iostream>

enum class Side{buy = 0, sell = 1};
enum class Command{add = 0, remove = 1};

struct Order
{ 
    int price;
    int quantity;
    int orderId;
};

struct ParsedInput
{
    Order orderData;
    Side side;
    Command command;
};

class OrderBook
{
    public:
        
        void printOrder(const int orderId);
        void OrderBook::orderOrder();
        void OrderBook::printAll(std::string& type);


    private:
        ParsedInput orderCreate(const std::string& line);
        void processOrder(const std::string& line);
        std::string getLine();

        std::vector<Order> bids;
        std::vector<Order> asks;
    
};

