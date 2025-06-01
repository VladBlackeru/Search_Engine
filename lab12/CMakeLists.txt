cmake_minimum_required(VERSION 3.28)
project(lab12)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab12 main.cpp
        Camera.cpp
        Camera.h
        Command.cpp
        Command.h
        Observer.cpp
        Observer.h
        Strategy.cpp
        Strategy.h
        Projector.cpp
        Projector.h)
