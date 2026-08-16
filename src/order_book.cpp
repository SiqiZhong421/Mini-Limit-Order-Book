#include "order_book.hpp"
#include <algorithm>
#include <iostream>

void OrderBook::incoming_pushback(const Order& order){
    Order incoming = order;

    if (incoming.side == Side::Buy) bids_[incoming.price].push_back(incoming);
    else                            asks_[incoming.price].push_back(incoming);
}

bool OrderBook::add_order(const Order& order){
    if(used_ids_.contains(order.id)){
        std::cerr << "Duplicate id!\n";
        return 0;
    }
    else if(order.id <= 0){
        std::cerr << "Wrong id!\n";
        return 0;
    }
    else if(order.price <= 0){
        std::cerr << "Wrong price!\n";
        return 0;
    }
    else if(order.quantity <= 0){
        std::cerr << "Wrong quantity!\n";
        return 0;
    }
    else{
        used_ids_.insert(order.id);

        Order incoming = order;
        
        while (incoming.quantity > 0){

            if (incoming.side == Side::Buy && !asks_.empty() ){
                
                auto best_ask = asks_.begin();
                double best_price = best_ask->first;
                int best_id = best_ask->second.front().id;

                if (best_price <= incoming.price) {

                    int volume = std::min(best_ask->second.front().quantity, incoming.quantity);

                    incoming.quantity -= volume;
                    best_ask->second.front().quantity -= volume;

                    trades_.push_back({incoming.id, best_id, best_price, volume });

                    if (best_ask->second.front().quantity == 0){

                        best_ask->second.pop_front();

                        if (best_ask->second.empty()) {

                            asks_.erase(best_ask);

                        }
                    }
                }
                else{

                    incoming_pushback(incoming);
                    break;

                }
            }

            else if (incoming.side == Side::Sell && !bids_.empty() ){
                
                auto best_bid = bids_.begin();
                double best_price = best_bid->first;
                int best_id = best_bid->second.front().id;

                if (best_price >= incoming.price) {

                    int volume = std::min(best_bid->second.front().quantity, incoming.quantity);

                    incoming.quantity -= volume;
                    best_bid->second.front().quantity -= volume;

                    trades_.push_back({incoming.id, best_id, best_price, volume });

                    if (best_bid->second.front().quantity == 0){
                        
                        best_bid->second.pop_front();

                        if (best_bid->second.empty()) {

                            bids_.erase(best_bid);

                        }
                    }
                }
                else{

                    incoming_pushback(incoming);
                    break;

                }
            }

            else {
                incoming_pushback(incoming);
                break;
            }
        }
    }
    return 1;
}

void OrderBook::print_orders() const{
    auto bids_it = bids_.cbegin();
    std::cout << "========== Bids ==========\n";

    while (bids_it != bids_.cend()){
        
        double price = bids_it->first;
        const std::deque<Order>& queue = bids_it->second;

        auto order_it = queue.cbegin();

        while (order_it != queue.cend() ){

            const Order& order = *order_it;
                
            std::cout << "Order id: "
                      << order.id
                      << ", side: Buy"
                      << ", price: "
                      << price
                      << ", quantity: "
                      << order.quantity
                      << '\n';
            
            order_it++;

        }

        bids_it++;

    }

    auto asks_it = asks_.cbegin();
    std::cout << "========== Asks ==========\n";

    while (asks_it != asks_.cend()) {

        double price = asks_it->first;
        const std::deque<Order>& queue = asks_it->second;

        auto order_it = queue.cbegin();

        while (order_it != queue.cend()) {

            const Order& order = *order_it;

            std::cout << "Order id: "
                      << order.id
                      << ", side: Sell"
                      << ", price: "
                      << price
                      << ", quantity: "
                      << order.quantity
                      << '\n';

            order_it++;
        }

        asks_it++;
        
    }
}

void OrderBook::print_trades() const{
    for (const Trade& trade : trades_){
        std::cout << "Incoming order "
                  << trade.incoming_order_id
                  << " traded with resting order "
                  << trade.resting_order_id
                  << " at price "
                  << trade.price
                  << " quantity "
                  << trade.quantity
                  << '\n';
    }
}

bool OrderBook::cancel_order(int order_id){
    for (auto bids_it = bids_.begin(); bids_it != bids_.end(); ++bids_it){

        std::deque<Order>& queue = bids_it->second;
        

        for (auto order_it = queue.begin(); order_it != queue.end(); ++order_it){

            if (order_it->id == order_id){

                queue.erase(order_it);

                if (queue.empty() ){

                    bids_.erase(bids_it);
                    
                }

                return 1;
            }
        }
    }

    for (auto asks_it = asks_.begin(); asks_it != asks_.end(); ++asks_it){

        std::deque<Order>& queue = asks_it->second;
        

        for (auto order_it = queue.begin(); order_it != queue.end(); ++order_it){

            if (order_it->id == order_id){

                queue.erase(order_it);

                if (queue.empty() ){

                    asks_.erase(asks_it);
                    
                }

                return 1;
            }
        }
    }

    return false;
}