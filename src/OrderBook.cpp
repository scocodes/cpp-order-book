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
        completedOrder.orderData.orderId = allocatedOrderId;
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

    for(int i = 0; i < bids.size(); i++)
    {
        if(bids[i].orderId == orderId)
        {
            bids.erase(bids.begin() + i);
            break;
        }
    }

    for(int i = 0; i < asks.size(); i++)
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

        if (!Input::inputFormat(newOrder))
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
                            recordTrade(existingOrder.quantity, existingOrder.price, existingOrder.orderId, orderInput.orderId);
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
                    recordTrade(existingOrder.quantity, existingOrder.price, existingOrder.orderId, orderInput.orderId);
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
                    recordTrade(orderInput.quantity, existingOrder.price, existingOrder.orderId, orderInput.orderId);
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
    });
        
    for(int i = 0; i < bids.size(); i++)
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

    for(int i = 0; i < bids.size(); i++)
    {
        if(asks[i].quantity <= 0)
        {
            asks.erase(asks.begin() + i);
        }
        else
        {
            break;
        }
    }

    std::sort(bids.begin(), bids.end(), [](const Order& a, const Order& b)
    {
        if(a.price != b.price)
        {
            return a.price > b.price;
        }
        return a.timestamp < b.timestamp;
    });
    std::sort(asks.begin(), asks.end(), [](const Order& a, const Order& b)
    {
        if(a.price != b.price)
        {
            return a.price < b.price;
        }
        return a.timestamp < b.timestamp;
    });
}

void OrderBook::recordTrade(int quantity, int salePrice, int buyerId, int sellerId)
{
    std::chrono::system_clock::time_point timeOfSale = std::chrono::system_clock::now();
    TradeHistory newTrade(quantity, salePrice, buyerId, sellerId, timeOfSale);
}
   


// When a sale is made, we need to understand the orderid of the seller and the buyer
// The quantity and price of the sale
// The time the transaction occured