cmake_minimum_required(VERSION 3.20)

project(order_book
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(order_book
    main.cpp
    src/OrderBook.cpp
)

target_include_directories(order_book
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(order_book
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)