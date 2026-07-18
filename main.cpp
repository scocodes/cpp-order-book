#include <iostream>
#include "orderBook.hpp"
#include <sstream>

int main()
{
    // "Format: ADD 101 BUY 10525 50"
    OrderBook book;
    book.orderOrder();

    std::string asks = "ASKS";
    std::string bids = "BIDS";
    std::string all = "ALL";

    book.printAll(asks);
    book.printAll(bids);
    book.printAll(all);
    
    return 0;
}
