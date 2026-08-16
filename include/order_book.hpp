#pragma once
#include <vector>
#include <unordered_set>
#include <map>
#include <deque>
#include <functional>

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

struct Trade{
    int incoming_order_id;
    int resting_order_id;
    double price;
    int quantity;
};

class OrderBook{
private:
    std::vector<Trade> trades_;
    std::unordered_set<int> used_ids_;

    std::map<double, std::deque<Order>, std::greater<double>> bids_;
    std::map<double, std::deque<Order>> asks_;

    void incoming_pushback(const Order& order);

public:
    bool add_order(const Order& order);
    bool cancel_order(int order_id);
    void print_orders() const;
    void print_trades() const;
};
