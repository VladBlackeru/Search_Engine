cmake_minimum_required(VERSION 3.28)
project(app)

set(CMAKE_CXX_STANDARD 17)

add_executable(app main.cpp)
include_directories("C:/Program Files/PostgreSQL/16/include")
link_directories("C:/Program Files/PostgreSQL/16/lib")
target_link_libraries(app "C:/Program Files/PostgreSQL/16/lib/libpq.lib")