#include <iostream>


enum Side{buy = 0, sell = 1};

struct Order
{
    int price;
    Side order;
    int quantity;
    int orderId;
};

