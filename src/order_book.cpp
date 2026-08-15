#include "order_book.hpp"
#include <iostream>
#include <algorithm>

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
            std::size_t best_order = orders_.size();

            for(std::size_t i = 0; i < orders_.size(); i++){

                Order& resting = orders_[i];

                if (can_match(incoming, resting)){
                    
                    if(best_order == orders_.size())  best_order = i;

                    else if(incoming.side == Side::Buy && resting.price < orders_[best_order].price) best_order = i;

                    else if(incoming.side == Side::Sell && resting.price > orders_[best_order].price) best_order = i;

                }
            }

            if (best_order == orders_.size()){
                orders_.push_back(incoming);
                break;
            }

            Order& resting = orders_[best_order];
            double best_price = resting.price;
            int volume = std::min(resting.quantity, incoming.quantity);

            incoming.quantity -= volume;
            resting.quantity -= volume;

            std::cout<<"Trade: price = "<<best_price<<", quantity = "<<volume<<std::endl;

            if(resting.quantity == 0){
                orders_.erase(orders_.begin() + best_order);
            }
        }
    }
    return 1;
}

void OrderBook::print_orders() const{
    for (const Order& order : orders_){
        std::cout<<order.id<<" "<<(order.side == Side::Buy ? "Buy" : "Sell")<<" "<<order.price<<" "<<order.quantity<<std::endl;
    }
}

bool OrderBook::can_match(const Order& incoming, const Order& resting) const{
    if (incoming.side == Side::Buy && resting.side == Side::Sell){
        return resting.price <= incoming.price;
    }

    if (incoming.side == Side::Sell && resting.side == Side::Buy){
        return resting.price >= incoming.price;
    }

    return false;
}

bool OrderBook::cancel_order(int order_id){
    for (std::size_t i = 0; i < orders_.size(); i++){

        if(orders_[i].id == order_id){
            orders_.erase(orders_.begin() + i);
            return true;
        }

    }
    return false;
}