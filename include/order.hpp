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

