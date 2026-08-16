cmake_minimum_required(VERSION 3.20)

project(MiniLimitOrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(order_book
    src/order_book.cpp
)

target_include_directories(order_book
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(order_book_demo
    src/main.cpp
)

target_link_libraries(order_book_demo
    PRIVATE
        order_book
)

add_executable(order_book_tests
    tests/test_order_book.cpp
)

include(CTest)

enable_testing()

add_test(
    NAME order_book_tests
    COMMAND order_book_tests
)

target_link_libraries(order_book_tests
    PRIVATE
        order_book
)