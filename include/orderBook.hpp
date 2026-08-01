#pragma once
#include <string>
#include <vector>

enum class Side{none, buy = 0, sell = 1};
enum class Command{none, add = 0, remove = 1};
enum class OrderType{none, market = 0, limit = 1};

struct Order
{ 
    int price;
    int quantity;
    int orderId;
    std::chrono::system_clock::time_point timestamp;
};

struct ParsedInput
{
    Order orderData;
    Side side;
    Command command;
    OrderType orderType;
};

struct TradeHistory
{
    int quantity{};
    int buyerId{};
    int sellerId{};
    int salePrice{};
    std::chrono::system_clock::time_point timeOfSale{};

    TradeHistory(int quantity, int salePrice, int buyerId, int sellerId, 
        std::chrono::system_clock::time_point timeOfSale) 
    : quantity(quantity), salePrice(salePrice), buyerId(buyerId), sellerId(sellerId),
    timeOfSale(timeOfSale)
    {
    }

};

class OrderBook
{
    public:
        
        void mainOrder();
        void printOrder(const int orderId) const;
        void printAll(const std::string& type) const;


    private:
        void processOrder(ParsedInput& newOrder);
        void addOrder(ParsedInput& order2Add);
        void removeOrder(ParsedInput& order2Remove);
        bool matchOrder(ParsedInput& order2Match);
        void eraseAndSort();
        void recordTrade(int quantity, int salePrice, int buyerId, int sellerId);

        std::vector<Order> bids;
        std::vector<Order> asks;
        
        std::vector<TradeHistory> tradeHistory;
        int allocatedOrderId{1};
    
};

