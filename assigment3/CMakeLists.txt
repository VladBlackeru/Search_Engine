cmake_minimum_required(VERSION 3.28)
project(assigment3)

set(CMAKE_CXX_STANDARD 17)

add_executable(assigment3 main.cpp
        Event.cpp
        Event.h
        EventStore.cpp
        EventStore.h
        OrderBook.cpp
        OrderBook.h
        AccountManager.cpp
        AccountManager.h
        CommandHandlers.cpp
        CommandHandlers.h
        Event.cpp
        Event.h)
