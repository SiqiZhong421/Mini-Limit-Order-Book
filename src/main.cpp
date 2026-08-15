#include "order_book.hpp"
#include <iostream>
#include <string>

int passed = 0;
int failed = 0;

void expect(bool actual,
            bool expected,
            const std::string& test_name) {
    if (actual == expected) {
        std::cout << "[PASS] "
                  << test_name
                  << '\n';
        ++passed;
    } else {
        std::cout << "[FAIL] "
                  << test_name
                  << '\n';
        ++failed;
    }
}

int main() {
    // 1. 正常订单应该被接受
    {
        OrderBook book;

        expect(
            book.add_order({1, Side::Buy, 100.0, 10}),
            true,
            "Accept valid order"
        );
    }

    // 2. 非法 ID
    {
        OrderBook book;

        expect(
            book.add_order({0, Side::Buy, 100.0, 10}),
            false,
            "Reject zero ID"
        );

        expect(
            book.add_order({-1, Side::Buy, 100.0, 10}),
            false,
            "Reject negative ID"
        );
    }

    // 3. 非法价格
    {
        OrderBook book;

        expect(
            book.add_order({1, Side::Buy, 0.0, 10}),
            false,
            "Reject zero price"
        );

        expect(
            book.add_order({2, Side::Buy, -100.0, 10}),
            false,
            "Reject negative price"
        );
    }

    // 4. 非法数量
    {
        OrderBook book;

        expect(
            book.add_order({1, Side::Buy, 100.0, 0}),
            false,
            "Reject zero quantity"
        );

        expect(
            book.add_order({2, Side::Buy, 100.0, -5}),
            false,
            "Reject negative quantity"
        );
    }

    // 5. 重复 ID
    {
        OrderBook book;

        expect(
            book.add_order({1, Side::Buy, 100.0, 10}),
            true,
            "Accept first use of ID"
        );

        expect(
            book.add_order({1, Side::Sell, 101.0, 5}),
            false,
            "Reject duplicate active ID"
        );
    }

    // 6. 撤单后也不能重复使用 ID
    {
        OrderBook book;

        book.add_order({1, Side::Buy, 100.0, 10});
        book.cancel_order(1);

        expect(
            book.add_order({1, Side::Sell, 101.0, 5}),
            false,
            "Reject reused cancelled ID"
        );
    }

    // 7. 成交订单仍然不能重复使用 ID
    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 5});

        expect(
            book.add_order({2, Side::Buy, 101.0, 5}),
            true,
            "Accept matching order"
        );

        expect(
            book.add_order({1, Side::Buy, 99.0, 5}),
            false,
            "Reject reused fully traded ID"
        );
    }

    // 8. 撤单成功和失败
    {
        OrderBook book;

        book.add_order({1, Side::Buy, 100.0, 10});

        expect(
            book.cancel_order(1),
            true,
            "Cancel existing order"
        );

        expect(
            book.cancel_order(999),
            false,
            "Reject cancelling missing order"
        );
    }

    // 9. 一个订单连续匹配多个订单
    {
        OrderBook book;

        book.add_order({1, Side::Sell, 100.0, 3});
        book.add_order({2, Side::Sell, 101.0, 4});

        expect(
            book.add_order({3, Side::Buy, 102.0, 10}),
            true,
            "Accept order matching multiple orders"
        );
    }

    std::cout << "\n====================\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}