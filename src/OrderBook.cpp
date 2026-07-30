#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "orderBook.hpp"

void OrderBook::processOrder(ParsedInput& completedOrder)
{ 
    if (completedOrder.command == Command::add)
    {
        if (completedOrder.side == Side::buy)
        {
            if(!matchOrder(completedOrder))
            {
                bids.push_back(completedOrder.orderData);
            }
        }
        else if (completedOrder.side == Side::sell)
        {
            if(!matchOrder(completedOrder))
            {
                asks.push_back(completedOrder.orderData);
            }
        }
    }

    else if (completedOrder.command == Command::remove)
    {
        int orderId = completedOrder.orderData.orderId;

        for(int i = 0; i< bids.size(); i++)
        {
            if(bids[i].orderId == orderId)
            {
                bids.erase(bids.begin() + i);
                break;
            }
        }
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
      

ParsedInput OrderBook::orderCreate(const std::string& line)
{
    std::istringstream iss(line);
    std::string command;
    std::string orderTypeText;
    int id;
    std::string side;
    int price;
    int quantity;
    Order orderInput{};
    ParsedInput completedOrder{};

    iss >> command;

    if (command == "ADD")
    {
        iss >> id >> side >> orderTypeText >> price >> quantity;

        Command orderType = Command::add;
        completedOrder.command = orderType;
        orderInput.orderId = id;
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

void OrderBook::mainOrder()
{
    while(true)
    {
        std::cout << "Please input your new order: " << "\n";
        std::string newOrder = getLine();

        if (!inputFormat(newOrder))
        {
            continue;
        }
        ParsedInput madeOrder = orderCreate(newOrder);
        processOrder(madeOrder);
    }
}

std::string OrderBook::getLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool OrderBook::inputFormat(const std::string& input)
{
    std::istringstream iss(input);
    std::string command;
    int id;
    std::string side;
    std::string orderTypeText;
    int price;
    int quantity;
    std::string extra;

    iss >> command;
    if (command == "ADD")
    {
        if(!(iss >> id >> side >> orderTypeText >> price >> quantity))
        {
            std::cout << "Missing Field or Incorrect Order" << "\n";
            return false;
        }
        if(iss >> extra)
        {
            return false;
            // Understand how iss works better
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


bool OrderBook::matchOrder(ParsedInput& order2Match)
{
    Order& orderInput = order2Match.orderData;
    Side& orderSide = order2Match.side;
    OrderType& oType = order2Match.orderType;

    eraseAndSort();

    if(oType == OrderType::limit)
    {
        if(orderSide == Side::buy)
        {
            for(std::size_t i = 0; i < asks.size(); i++)
                {
                    if(orderInput.price >= asks[i].price && orderInput.quantity > 0)
                    {
                        if(orderInput.quantity >= asks[i].quantity)
                        {
                            orderInput.quantity -= asks[i].quantity;
                            asks[i].quantity = 0;
                            if(orderInput.quantity > 0)
                            {
                                std::cout << "Order: " << asks[i].orderId << "has been fulfilled. " << "\n";
                            }
                            else
                            {
                                std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                                return true;
                            }

                        }
                        else if(asks[i].quantity > orderInput.quantity)
                        {
                            asks[i].quantity -= orderInput.quantity;
                            orderInput.quantity = 0;
                            std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                            return true;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                if(orderInput.quantity > 0)
                {
                    return false;
                }
        }
        
        else if(orderSide == Side::sell)
        {
            for(std::size_t i = 0; i < bids.size(); i++)
                {
                    if(orderInput.price <= bids[i].price && orderInput.quantity > 0)
                    {
                        if(orderInput.quantity >= bids[i].quantity)
                        {
                            orderInput.quantity -= bids[i].quantity;
                            bids[i].quantity = 0;

                            if(orderInput.quantity > 0)
                            {
                                std::cout << "Order: " << bids[i].orderId << "has been fulfilled. " << "\n";
                            }
                            else
                            {
                                std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                                return true;
                            }

                        }
                        else if(bids[i].quantity > orderInput.quantity)
                        {
                            bids[i].quantity -= orderInput.quantity;
                            orderInput.quantity = 0;
                            std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                            return true;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            if(orderInput.quantity > 0 )
            {
                return false;
            }
        }
    }

    else if (oType == OrderType::market)
    {
    int avrPrices = 0;
    int avrOrders = 0;
    int avrOrderQuant = 0;
        if(orderSide == Side::buy)
        {
            

            for(std::size_t i = 0; i < asks.size(); i++)
            {
                if(orderInput.quantity >= asks[i].quantity && orderInput.quantity > 0 )
                {
                    avrOrderQuant += asks[i].quantity;

                    orderInput.quantity -= asks[i].quantity;
                    asks[i].quantity = 0;
                    if(orderInput.quantity > 0)
                    {
                        std::cout << "Order: " << asks[i].orderId << "has been fulfilled. " << "\n";
                        avrPrices += avrOrderQuant * asks[i].price;
                        avrOrders ++;
                    }

                    else if(avrOrders > 1)
                    {
                        avrPrices /= orderInput.quantity;
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at an average price of: " << avrPrices << "\n";
                        return true;
                    }

                    else
                    {
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << asks[i].price << "\n";
                        return true;
                    }
                }
                else if(asks[i].quantity > orderInput.quantity)
                {
                    asks[i].quantity -= orderInput.quantity;
                    orderInpu.quantity = 0;
                    std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << asks[i].price << "\n";
                    return true;
                }
            }
            if(orderInput.quantity > 0)
            {
                return false;
            }
        }

        else if(orderSide == Side::sell)
        {
            for(std::size_t i = 0; i < bids.size(); i++)
            {
                if(orderInput.quantity >= bids[i].quantity)
                {
                    avrOrderQuant += bids[i].quantity;

                    orderInput.quantity -= bids[i].quantity;
                    bids[i].quantity = 0;

                    if(orderInput.quantity > 0)
                    {
                        std::cout << "Order: " << bids[i].orderId << "has been fulfilled. " << "\n";
                        avrPrices += avrOrderQuant * bids[i].price;
                        avrOrders ++;
                    }

                    else if(avrOrders > 1)
                    {
                        avrPrices /= orderInput.quantity;
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at an average price of: " << avrPrices << "\n";
                        return true;
                    }

                    else
                    {
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << bids[i].price << "\n";
                        return true;
                    }
                }
                else if(bids[i].quantity > orderInput.quantity)
                {
                    bids[i].quantity -= orderInput.quantity;
                    orderInput.quantity = 0;
                    std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << bids[i].price << "\n";
                    return true;
                }
            }
            if (orderInput.quantity > 0)
            {
                return false;
            }
        }
    
    }

    eraseAndSort();
    return false;
}

void OrderBook::eraseAndSort()
{
    std::sort(bids.begin(), bids.end(), quantitySort);
    std::sort(asks.begin(), asks.end(), quantitySort);

    for(std::size_t i = 0; i < bids.size(); i++)
    {
        if(bids[i].quantity <= 0)
        {
            bids.erase(bids.begin() + i);
        }
        else
        {
            break;
        }
    }
    for(std::size_t i = 0; i < asks.size(); i++)
        if(asks[i].quantity <= 0)
        {
            asks.erase(asks.begin() + i);
        }
        else
        {
            break;
        }
    std::sort(bids.begin(), bids.end(), comparisonBuy);
    std::sort(asks.begin(), asks.end(), comparisonSell);
}


bool OrderBook::comparisonBuy(const Order& a, const Order& b)
{
    return a.price > b.price;
}

bool OrderBook::comparisonSell(const Order& a, const Order& b)
{
    return a.price < b.price;
}

bool OrderBook::quantitySort(const Order& a, const Order& b)
{
    return a.quantity < b.quantity;
}

// Rewrite the matchOrder to, if the order is not filled it needs to be added to the orderbook
// Order needs to check conditions before looping through everyorder in the book, unless it is a market order
// Limit order should check first if the price is relevant, if the price is not relevant and the book is sorted then it should be added to the order book
// main function should function as the input is given, checked, ran through the matching algorithm.
