# Project
cmake_minimum_required(VERSION 3.14)

project(Queue LANGUAGES C CXX)

# Options
option(QUEUE_TESTING "Включить модульное тестирование" ON)

option(ENABLE_WARNING "Включить предупреждения" OFF)

# Warnings
if (ENABLE_WARNING)
    if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        add_compile_options(
            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            -Wconversion
            -Wctor-dtor-privacy
            -Wenum-compare
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wsign-conversion
            -Wsign-promo
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(/W4 /WX)
    endif()

    if(NOT CMAKE_CXX_EXTENSIONS)
        set(CMAKE_CXX_EXTENSIONS OFF)
    endif()
endif()   

# Headers
add_library(Queue_headers INTERFACE)
target_include_directories(Queue_headers INTERFACE include)
add_library(Queue::headers ALIAS Queue_headers)

# Source
add_library(Queue_source STATIC 
    src/p_queue.c
)

target_link_libraries(Queue_source PUBLIC Queue_headers)
target_compile_features(Queue_source PRIVATE c_std_17)

add_library(Queue::source ALIAS Queue_source)

# Testing
if(NOT QUEUE_TESTING)
    message(STATUS "Тестирование выключено")
else()
    add_subdirectory(test)
endif()

