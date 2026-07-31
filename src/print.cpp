#include <iostream>
#include <algorithm>
#include <vector>
#include "orderBook.hpp"


void OrderBook::printOrder(const int orderId) const
{
    for(std::size_t i = 0; i < bids.size(); i++)
    {
        if(bids[i].orderId == orderId)
        {
            std::cout << "You ordered: " << "\n" <<
            bids[i].orderId << " " << bids[i].quantity << " " << bids[i].price << "\n";
            break;
        }
    }
    for(std::size_t i = 0; i < asks.size(); i++)
    {
        if(asks[i].orderId == orderId)
        {
            std::cout << "You want to sell: " << "\n" <<
            asks[i].orderId << " " << asks[i].quantity << " " << asks[i].price << "\n";
            break;
        }
    }
}

void OrderBook::printAll(const std::string& type) const
{
    if(type == "BIDS")
    {
        for(std::size_t i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
    }
    else if (type == "ASKS")
    {
        for(std::size_t i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << asks[i].orderId << " " << asks[i].price << " " << asks[i].quantity << "\n";
        }  
    }
    else if (type == "ALL")
    {
        for(std::size_t i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
        for(std::size_t i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << asks[i].orderId << " " << asks[i].price << " " << asks[i].quantity << "\n";
        }  
    }
}
      