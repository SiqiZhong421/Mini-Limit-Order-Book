#include "order_book.hpp"
#include <iostream>

int main() {
    OrderBook book;

    book.add_order({1, Side::Sell, 100.0, 5});
    book.add_order({2, Side::Buy, 101.0, 3});

    book.print_orders();

    return 0;
}