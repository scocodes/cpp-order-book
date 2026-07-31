#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "orderBook.hpp"
#include "input.hpp"

std::string Input::getLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool Input::inputFormat(const std::string& input)
{
    std::istringstream iss(input);
    std::string command;
    std::string side;
    int id{};
    std::string orderTypeText;
    int price{};
    int quantity{};
    std::string extra;

    iss >> command;
    if (command == "ADD")
    {
        if(!(iss >> side >> orderTypeText >> price >> quantity))
        {
            std::cout << "Missing Field or Incorrect Order" << "\n";
            return false;
        }
        if(iss >> extra)
        {
            return false;
            // Understand how iss works better
        }
        else{
            return true;
        }
    }

    else if (command == "REMOVE")
    {
        if(!(iss >> id))
        {
            std::cout << "Missing Field or Incorrect Order" << "\n";
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

ParsedInput Input::orderCreate(const std::string& line)
{
    std::istringstream iss(line);
    std::string command;
    std::string orderTypeText;
    int id{};
    std::string side;
    int price{};
    int quantity{};
    Order orderInput{};
    ParsedInput completedOrder{};

    iss >> command;

    if (command == "ADD")
    {
        iss >> side >> orderTypeText >> price >> quantity;

        Command orderType = Command::add;
        completedOrder.command = orderType;
        orderInput.price = price;
        orderInput.quantity = quantity;

            if (orderTypeText == "LIMIT")
            {
                OrderType tyOrd = OrderType::limit;
                completedOrder.orderType = tyOrd;
            }
            else if (orderTypeText == "MARKET")
            {
                OrderType tyOrd = OrderType::market;
                completedOrder.orderType = tyOrd;
            }
    
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
        iss >> id;
        orderInput.orderId = id;

        Command orderType = Command::remove;
        completedOrder.command = orderType;
    }

    completedOrder.orderData = orderInput;
    return completedOrder;
}
