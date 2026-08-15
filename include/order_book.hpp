#pragma once
#include <vector>
#include <unordered_set>

enum class Side{
    Buy,
    Sell
};

struct Order{
    int id;
    Side side;
    double price;
    int quantity;
};

class OrderBook{
private:
    std::vector<Order> orders_;
    std::unordered_set<int> used_ids_;
    bool can_match(const Order& incoming, const Order& resting) const;

public:
    bool add_order(const Order& order);
    void print_orders() const;
    bool cancel_order(int order_id);
};
