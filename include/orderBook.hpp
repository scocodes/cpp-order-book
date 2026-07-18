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
        void orderOrder();
        void printAll(const std::string& type);


    private:
        ParsedInput orderCreate(const std::string& line);
        void processOrder(const std::string& line);
        std::string getLine();
        bool inputFormat(std::string input);
        void OrderBook::matchOrder(Order& orderInput);
        bool comparisonBuy(const Order& a, const Order& b);
        bool comparisonSell(const Order& a, const Order& b);
        void OrderBook::pushAndSort();

        std::vector<Order> bids;
        std::vector<Order> asks;
    
};

