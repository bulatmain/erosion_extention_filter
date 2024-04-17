cmake_minimum_required(VERSION 3.14)

project(DPP LANGUAGES C CXX)

option(DPP_TESTING "Включить модульное тестирование" ON)

option(QUEUE_TESTING "Включить модульное тестирование модуля p_queue" OFF)

option(ENABLE_WARNING "Включить предупреждения" OFF)

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

add_compile_options(
    -g
)

add_subdirectory(include/p_queue)

add_library(DPP_headers INTERFACE)
target_include_directories(DPP_headers INTERFACE include/lib)
add_library(DPP::headers ALIAS DPP_headers)

add_library(DPP_source STATIC
    src/planner.c
    src/planner_aux.c
    src/filter.c
    src/tasks.c
)
target_link_libraries(DPP_source PUBLIC DPP_headers)
target_link_libraries(DPP_source PUBLIC Queue::source)
target_compile_features(DPP_source PRIVATE c_std_17)

add_library(DPP::source ALIAS DPP_source)

add_executable(exec exec/main.c)
target_link_libraries(exec PRIVATE DPP_source)

# if(NOT DPP_TESTING)
#     message(STATUS "Тестирование выключено")
# else()
#     add_subdirectory(test)
# endif()

