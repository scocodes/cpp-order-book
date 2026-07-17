#include <iostream>
#include "order.hpp"
#include <sstream>

std::string getLine();

int main()
{
    // "Format: ADD 101 BUY 10525 50"
    OrderBook book;
    std::string newOrder = getLine();
    book.addOrder(newOrder);
    book.print("BIDS");
    book.print("ASKS");
    std::string newOrder2 = getLine();
    book.addOrder(newOrder2);
    book.print("BIDS");
    book.print("ASKS");
    book.removeOrder(newOrder);
    book.print("BIDS");
    book.print("ASKS");
    return 0;
}

std::string getLine()
    {
        std::string line;
        std::getline(std::cin, line);

        return line;
    };
