#pragma once
#include <iostream>

enum class Side{buy = 0, sell = 1};
enum class Command{add = 0, remove = 1};

struct Order
{ 
    int price;
    Side side;
    int quantity;
    int orderId;
};

struct ParsedInput{
    Order orderData;
    Command command;
};

