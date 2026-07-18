#include <iostream>
#include <vector>
#include <sstream>
#include "orderBook.hpp"


void OrderBook::processOrder(const std::string& line)
{
    ParsedInput completedOrder = orderCreate(line);
    if (completedOrder.command == Command::add)
    {
        if (completedOrder.side == Side::buy)
        {
            bids.push_back(completedOrder.orderData);
        }
        else if (completedOrder.side == Side::sell)
        {
            asks.push_back(completedOrder.orderData);
        }
    }
    else if (completedOrder.command == Command::remove)
    {
        int orderId = completedOrder.orderData.orderId;

        if (completedOrder.side == Side::buy)
        {
            for(int i = 0; i< bids.size(); i++)
            {
                if(bids[i].orderId == orderId)
                {
                    bids.erase(bids.begin() + i);
                    break;
                }
            }
            }
        
        if (completedOrder.side == Side::sell)
        {
            for(int i = 0; i< asks.size(); i++)
            {
                if(asks[i].orderId == orderId)
                {
                    asks.erase(asks.begin() + i);
                    break;
                }
            } 

        }

    }
}

// Make orderId the input and then print function understands what to ouput 

void OrderBook::printOrder(const int orderId)
{
    for(int i = 0; i < bids.size(); i++)
    {
        if(bids[i].orderId == orderId)
        {
            std::cout << "You ordered: " << "\n" <<
            bids[i].orderId << " " << bids[i].quantity << " " << bids[i].price << "\n";
            break;
        }
    }
    for(int i = 0; i < asks.size(); i++)
    {
        if(asks[i].orderId == orderId)
        {
            std::cout << "You want to sell: " << "\n" <<
            asks[i].orderId << " " << asks[i].quantity << " " << asks[i].price << "\n";
            break;
        }
    }
}

void OrderBook::printAll(const std::string& type)
{
    if(type == "BIDS")
    {
        for(int i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
    }
    else if (type == "ASKS")
    {
        for(int i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << asks[i].orderId << " " << asks[i].price << " " << asks[i].quantity << "\n";
        }  
    }
    else if (type == "ALL")
    {
        for(int i = 0; i < bids.size(); i++)
        {
            std::cout << "BIDS: " << "\n" << bids[i].orderId << " " << bids[i].price << " " << bids[i].quantity << "\n";
        }
        for(int i = 0; i < asks.size(); i++)
        {
            std::cout << "ASKS: " << "\n" << asks[i].orderId << " " << asks[i].price << " " << asks[i].quantity << "\n";
        }  
    }
}
      

ParsedInput OrderBook::orderCreate(const std::string& line)
{
    std::istringstream iss(line);
    std::string command;
    int id;
    std::string side;
    int price;
    int quantity;
    Order orderInput;
    ParsedInput completedOrder;

    iss >> command;

    if (command == "ADD")
    {
        iss >> id >> side >> price >> quantity;
        orderInput.orderId = id;
        orderInput.price = price;
        orderInput.quantity = quantity;

        Command orderType = Command::add;
        completedOrder.command = orderType;
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

void OrderBook::orderOrder()
{
    while(true)
    {
    std::cout << "Please input your new order: " << "\n";
    std::string newOrder = getLine();
    if (!inputFormat(newOrder))
    {
        break;
    }
    if (!newOrder.empty())
    {
        processOrder(newOrder);
    }
    else if(newOrder.empty())
    {
        std::cout << "Please input your new order: " << "\n";
        std::string newOrder2 = getLine();
        if (newOrder2.empty())
        {
            break;
        }
        processOrder(newOrder2);
    }
}
}


std::string OrderBook::getLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool OrderBook::inputFormat(std::string input)
{
    std::istringstream iss(input);
    std::string command;
    int id;
    std::string side;
    int price;
    int quantity;
    std::string extra;

    iss >> command;
    if (command == "ADD")
    {
        if(!(iss >> id >> side >> price >> quantity))
        {
            std::cout << "Missing Field or Incorrect Order" << "\n";
            return false;
        }
        else if(iss >> id >> side >> price >> quantity >> extra)
        {
            return false;
        }
        else if(iss >> id >> side >> price >> quantity)
        {
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

// What does order book need to do? Assuming an input of an order, needs to understand what the type of order is
// Then it needs to search for the lowest asks or the highest bid price, if the right trade exists, low enough ask or high
// enough bid, execute trade
// trade execution, take quantity from taker and maker, if equal delete order, if unequal update quantities as so
// resort orders

void OrderBook::matchOrder(Order& orderInput)
{
    std::sort(bids.begin(), bids.end(), comparisonBuy);
    std::sort(asks.begin(), asks.end(), comparisonSell);

    // Buy

    for(int i = 0; i < asks.size(); i++)
    {
        if(orderInput.price >= asks[i].price && orderInput.quantity > 0)
        {
            if(orderInput.quantity >= asks[i].quantity)
            {
                orderInput.quantity -= asks[i].quantity;
                if(orderInput.quantity > 0)
                {
                    std::cout << "Order: " << asks[i].orderId << "has been fufilled. " << "\n";
                }
                else
                {
                    std::cout << "Order: " << orderInput.orderId << "has been fufilled. " << "\n";
                    break;
                }

            }
            else if(asks[i].quantity > orderInput.quantity)
            {
                orderInput.quantity -= asks[i].quantity;
                std::cout << "Order: " << orderInput.orderId << "has been fufilled. " << "\n";
            }
        }
    }

    // Sell

    for(int i = 0; i < bids.size(); i++)
    {
        if(orderInput.price <= bids[i].price && orderInput.quantity > 0)
        {
            if(orderInput.quantity >= bids[i].quantity)
            {
                orderInput.quantity -= bids[i].quantity;
                
                if(orderInput.quantity > 0)
                {
                    std::cout << "Order: " << bids[i].orderId << "has been fufilled. " << "\n";
                }
                else
                {
                    std::cout << "Order: " << orderInput.orderId << "has been fufilled. " << "\n";
                    break;
                }

            }
            else if(bids[i].quantity > orderInput.quantity)
            {
                orderInput.quantity -= bids[i].quantity;
                std::cout << "Order: " << orderInput.orderId << "has been fufilled. " << "\n";
            }
        }
    }



}
void OrderBook::pushAndSort()
{

}
bool OrderBook::comparisonBuy(const Order& a, const Order& b)
{
    return a.price > b.price;
}

bool OrderBook::comparisonSell(const Order& a, const Order& b)
{
    return a.price < b.price;
}