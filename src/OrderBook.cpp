#include <vector>
#include <sstream>
#include <algorithm>
#include "orderBook.hpp"
#include "input.hpp"
#include <iostream>

void OrderBook::processOrder(ParsedInput& completedOrder)
{ 
    if (completedOrder.command == Command::add)
    {
        order2Add.orderData.orderId = allocatedOrderId;
        allocatedOrderId ++;
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
            order2Add.orderData.timestamp = std::chrono::system_clock::now();
            bids.push_back(order2Add.orderData);
        }
    }
    else if (order2Add.side == Side::sell)
    {
        if(!matchOrder(order2Add))
        {
            order2Add.orderData.timestamp = std::chrono::system_clock::now();
            asks.push_back(order2Add.orderData);
        }
    }
}

void OrderBook::removeOrder(ParsedInput& order2Remove)
{
    int orderId = order2Remove.orderData.orderId;

    for(Order& order : bids)
    {
        if(order.orderId == orderId)
        {
            bids.erase(bids.begin() + i);
            break;
        }
    }

    for(Order& order : asks)
    {
        if(order.orderId == orderId)
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
            for(Order& existingOrder : asks)
                {
                    if(orderInput.price >= existingOrder.price && orderInput.quantity > 0)
                    {
                        if(orderInput.quantity >= existingOrder.quantity)
                        {
                            recordTrade(existingOrder.quantity, existingOrder.price, orderInput.orderId, existingOrder.orderId);
                            orderInput.quantity -= existingOrder.quantity;
                            existingOrder.quantity = 0;
                            if(orderInput.quantity > 0)
                            {
                                std::cout << "Order: " << existingOrder.orderId << "has been fulfilled. " << "\n";
                            }
                            else
                            {
                                std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                                return true;
                            }

                        }
                        else if(existingOrder.quantity > orderInput.quantity)
                        {
                            recordTrade(orderInput.quantity, orderInput.price, orderInput.orderId, existingOrder.orderId);
                            existingOrder.quantity -= orderInput.quantity;
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
            for(Order& existingOrder : bids)
                {
                    if(orderInput.price <= existingOrder.price && orderInput.quantity > 0)
                    {
                        if(orderInput.quantity >= existingOrder.quantity)
                        {
                            recordTrade(existingOrder.quantity, existingOrder.price, existingOrder.orderId, orderInput.orderId)
                            orderInput.quantity -= existingOrder.quantity;
                            existingOrder.quantity = 0;

                            if(orderInput.quantity > 0)
                            {
                                std::cout << "Order: " << existingOrder.orderId << "has been fulfilled. " << "\n";
                            }
                            else
                            {
                                std::cout << "Order: " << orderInput.orderId << "has been fulfilled. " << "\n";
                                return true;
                            }

                        }
                        else if(existingOrder.quantity > orderInput.quantity)
                        {
                            recordTrade(orderInput.quantity, orderInput.price, existingOrder.orderId, orderInput.orderId);
                            existingOrder.quantity -= orderInput.quantity;
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
            

            for(Order& existingOrder : asks)
            {
                if(orderInput.quantity >= existingOrder.quantity && orderInput.quantity > 0 )
                {
                    avrOrderQuant += existingOrder.quantity;
                    recordTrade(existingOrder.quantity, existingOrder.price, orderInput.orderId, existingOrder.orderId);
                    orderInput.quantity -= existingOrder.quantity;
                    existingOrder.quantity = 0;
                    if(orderInput.quantity > 0)
                    {
                        std::cout << "Order: " << existingOrder.orderId << "has been fulfilled. " << "\n";
                        avrPrices += avrOrderQuant * existingOrder.price;
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
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << existingOrder.price << "\n";
                        return true;
                    }
                }
                else if(existingOrder.quantity > orderInput.quantity)
                {
                    recordTrade(orderInput.quantity, existingOrder.price, orderInput.orderId, existingOrder.orderId);
                    existingOrder.quantity -= orderInput.quantity;
                    orderInput.quantity = 0;
                    std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << existingOrder.price << "\n";
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
            for(Order& existingOrder : bids)
            {
                if(orderInput.quantity >= existingOrder.quantity)
                {
                    avrOrderQuant += existingOrder.quantity;
                    recordTrade(existingOrder.quantity, existingOrder.price, exitingOrder.orderId, orderInput.orderId);

                    orderInput.quantity -= existingOrder.quantity;
                    existingOrder.quantity = 0;

                    if(orderInput.quantity > 0)
                    {
                        std::cout << "Order: " << existingOrder.orderId << "has been fulfilled. " << "\n";
                        avrPrices += avrOrderQuant * existingOrder.price;
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
                        std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << existingOrder.price << "\n";
                        return true;
                    }
                }
                else if(existingOrder.quantity > orderInput.quantity)
                {
                    recordTrade(orderInput.quantity, existingOrder.price, exitingOrder.orderId, orderInput.orderId);
                    existingOrder.quantity -= orderInput.quantity;
                    orderInput.quantity = 0;
                    std::cout << "Order: " << orderInput.orderId << "has been fulfilled at: " << existingOrder.price << "\n";
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
        
    for(Order& order : bids)
    {
        if(order.quantity <= 0)
        {
            bids.erase(bids.begin() + i);
        }
        else
        {
            break;
        }
    }
    for(Order& order : asks)
        if(order.quantity <= 0)
        {
            asks.erase(asks.begin() + i);
        }
        else
        {
            break;
        }
    std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b)
    {
        if(a.price != b.price)
        {
            return a.price > b.price;
        }
        return a.timestep < b.timestep;
    });
    std::sort(asks.begin(), asks.end(), [](const Order& a, const Order& b)
    {
        if(a.price != b.price)
        {
            return a.price < b.price;
        }
        return a.timestep < b.timestep;
    });
}

void OrderBook::recordTrade(int quantity, int salePrice, int buyerId, int sellerId)
{
    TradeHistory newTrade{};
    newTrade.quantity = quantity;
    newTrade.buyerId = orderId;
    newTrade.sellerId = orderId;
    newTrade.salePrice = price;
    newTrade.timeOfSale = std::chrono::system_clock::now();
}
   


// When a sale is made, we need to understand the orderid of the seller and the buyer
// The quantity and price of the sale
// The time the transaction occured