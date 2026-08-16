#include "order_book.hpp"
#include <iostream>
#include <cmath>

int failures = 0;

void expect(bool condition, const char* name) {
    if (!condition) {
        std::cerr << "[FAIL] " << name << '\n';
        ++failures;
    }
}

int main() {
// 11. 空订单簿没有 spread
{
    OrderBook book;

    expect(
        !book.spread(),
        "Empty book has no spread"
    );
}

// 12. 只有买单时没有 spread
{
    OrderBook book;

    book.add_order({1, Side::Buy, 100.0, 5});

    expect(
        !book.spread(),
        "Book with only bids has no spread"
    );
}

// 13. 只有卖单时没有 spread
{
    OrderBook book;

    book.add_order({1, Side::Sell, 105.0, 5});

    expect(
        !book.spread(),
        "Book with only asks has no spread"
    );
}

// 14. 正常计算 best ask - best bid
{
    OrderBook book;

    book.add_order({1, Side::Buy, 99.0, 5});
    book.add_order({2, Side::Buy, 100.0, 5});

    book.add_order({3, Side::Sell, 105.0, 5});
    book.add_order({4, Side::Sell, 103.0, 5});

    auto current_spread = book.spread();

    expect(
        current_spread.has_value(),
        "Spread exists when both sides exist"
    );

    expect(
        current_spread &&
        std::abs(*current_spread - 3.0) < 1e-9,
        "Spread equals best ask minus best bid"
    );
}

// 15. 撤销最佳买单后 spread 更新
{
    OrderBook book;

    book.add_order({1, Side::Buy, 100.0, 5});
    book.add_order({2, Side::Buy, 99.0, 5});
    book.add_order({3, Side::Sell, 105.0, 5});

    expect(
        book.spread() &&
        std::abs(*book.spread() - 5.0) < 1e-9,
        "Initial spread is correct"
    );

    book.cancel_order(1);

    expect(
        book.spread() &&
        std::abs(*book.spread() - 6.0) < 1e-9,
        "Spread updates after best bid cancellation"
    );
}

// 16. 撤销最佳卖单后 spread 更新
{
    OrderBook book;

    book.add_order({1, Side::Buy, 100.0, 5});
    book.add_order({2, Side::Sell, 103.0, 5});
    book.add_order({3, Side::Sell, 104.0, 5});

    expect(
        book.spread() &&
        std::abs(*book.spread() - 3.0) < 1e-9,
        "Initial best ask is correct"
    );

    book.cancel_order(2);

    expect(
        book.spread() &&
        std::abs(*book.spread() - 4.0) < 1e-9,
        "Spread updates after best ask cancellation"
    );
}

    if (failures == 0) {
        std::cout << "All spread tests passed.\n";
        return 0;
    }

    std::cerr << failures << " test checks failed.\n";
    return 1;
}