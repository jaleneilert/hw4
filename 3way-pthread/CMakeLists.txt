cmake_minimum_required (VERSION 2.8)
project(HW4)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror -D_XOPEN_SOURCE=500")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror -Wno-sign-compare -D_XOPEN_SOURCE=500")

include_directories("${PROJECT_SOURCE_DIR}/include")

# I don't think we need libraries this time


# make an executable
add_executable(${PROJECT_NAME}_test include/pthread_main.c)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE)

