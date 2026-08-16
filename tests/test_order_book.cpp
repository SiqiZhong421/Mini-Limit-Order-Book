#include "order_book.hpp"
#include <iostream>

void separator(const char* name) {
    std::cout << "\n========== "
                << name
                << " ==========\n";
}

int main() {
    // Test 1: 买卖订单分别进入订单簿
    separator("Test 1: Store unmatched orders");

    {
        OrderBook book;

        book.add_order({1, Side::Buy, 100.0, 10});
        book.add_order({2, Side::Sell, 105.0, 5});

        book.print_orders();
    }

    // Test 2: 买单应该优先匹配最低卖价
    separator("Test 2: Best ask priority");

    {
        OrderBook book;

        // 故意先加入较高价格
        book.add_order({1, Side::Sell, 101.0, 5});
        book.add_order({2, Side::Sell, 100.0, 5});

        book.add_order({3, Side::Buy, 102.0, 5});

        book.print_trades();
        book.print_orders();
    }

    // Test 3: 卖单应该优先匹配最高买价
    separator("Test 3: Best bid priority");

    {
        OrderBook book;

        // 故意先加入较低价格
        book.add_order({1, Side::Buy, 99.0, 5});
        book.add_order({2, Side::Buy, 100.0, 5});

        book.add_order({3, Side::Sell, 98.0, 5});

        book.print_trades();
        book.print_orders();
    }

    // Test 4: 同一价格下 FIFO
    separator("Test 4: FIFO at same price");

    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 3});
        book.add_order({2, Side::Sell, 100.0, 4});

        book.add_order({3, Side::Buy, 100.0, 5});

        book.print_trades();
        book.print_orders();
    }

    // Test 5: 一个买单匹配多个价格档位
    separator("Test 5: Multiple ask levels");

    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 3});
        book.add_order({2, Side::Sell, 101.0, 4});
        book.add_order({3, Side::Sell, 102.0, 5});

        book.add_order({4, Side::Buy, 105.0, 10});

        book.print_trades();
        book.print_orders();
    }

    // Test 6: 一个卖单匹配多个价格档位
    separator("Test 6: Multiple bid levels");

    {
        OrderBook book;

        book.add_order({1, Side::Buy, 102.0, 3});
        book.add_order({2, Side::Buy, 101.0, 4});
        book.add_order({3, Side::Buy, 100.0, 5});

        book.add_order({4, Side::Sell, 99.0, 10});

        book.print_trades();
        book.print_orders();
    }

    // Test 7: 部分成交后保留订单
    separator("Test 7: Partial fill");

    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 10});
        book.add_order({2, Side::Buy, 100.0, 4});

        book.print_trades();
        book.print_orders();
    }

    // Test 8: 价格档位完全清空
    separator("Test 8: Remove empty price level");

    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 5});
        book.add_order({2, Side::Buy, 100.0, 5});

        book.print_trades();
        book.print_orders();
    }

    // Test 9: 撤销买单
    separator("Test 9: Cancel bid");

    {
        OrderBook book;

        book.add_order({1, Side::Buy, 100.0, 5});

        std::cout << "Cancel result: "
                  << book.cancel_order(1)
                  << '\n';

        book.print_orders();
    }

    // Test 10: 撤销卖单
    separator("Test 10: Cancel ask");

    {
        OrderBook book;

        book.add_order({1, Side::Sell, 101.0, 5});

        std::cout << "Cancel result: "
                  << book.cancel_order(1)
                  << '\n';

        book.print_orders();
    }

    return 0;
}