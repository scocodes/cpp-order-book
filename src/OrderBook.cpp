#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "orderBook.hpp"
#include "input.hpp"

void OrderBook::processOrder(ParsedInput& completedOrder)
{ 
    if (completedOrder.command == Command::add)
    {
        addOrder(completedOrder);
    }

    else if (completedOrder.command == Command::remove)
    {
        removeOrder(completedOrder);
    }
    
}

void OrderBook::addOrder(ParsedInput& order2Add)
{
    if (order2Add.side == Side::buy)
    {
        if(!matchOrder(order2Add))
        {
            bids.push_back(order2Add.orderData);
        }
    }
    else if (order2Add.side == Side::sell)
    {
        if(!matchOrder(order2Add))
        {
            asks.push_back(order2Add.orderData);
        }
    }
}

void OrderBook::removeOrder(ParsedInput& order2Remove)
{
    int orderId = order2Remove.orderData.orderId;

    for(std::size_t i = 0; i< bids.size(); i++)
    {
        if(bids[i].orderId == orderId)
        {
            bids.erase(bids.begin() + i);
            break;
        }
    }

    for(std::size_t i = 0; i< asks.size(); i++)
    {
        if(asks[i].orderId == orderId)
        {
            asks.erase(asks.begin() + i);
            break;
        }
    } 
}

void OrderBook::mainOrder()
{
    while(true)
    {
        std::cout << "Please input your new order: " << "\n";
        std::string newOrder = Input::getLine();

        if (!inputFormat(newOrder))
        {
            continue;
        }
        ParsedInput madeOrder = Input::orderCreate(newOrder);
        processOrder(madeOrder);
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
                    orderInput.quantity = 0;
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
    std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b)
    {
        return a.quantity < b.quantity;
    })
        
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
    std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b)
    {
        return a.price > b.price;
    });
    std::sort(asks.begin(), asks.end(), [](const Order& a, const Order& b)
    {
        return a.price < b.price;
    });
}

// Add timestep data and priority for first come first served
// Add automatic orderId assignment
