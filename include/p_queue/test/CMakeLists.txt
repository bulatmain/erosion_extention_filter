function(OPTION_TURN_ON_TESTING 
    CONDITION TEST_NAME TEST_SOURCE EXEC_TARGET_NAME MESSAGE)
    if (NOT ${CONDITION})
        message(${MESSAGE})
    else()
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} 
            PRIVATE
                Queue_source
                GTest::GTest
        )
        add_custom_target(${EXEC_TARGET_NAME} ALL COMMAND ${TEST_NAME})
    endif()    

endfunction()

include(${PROJECT_SOURCE_DIR}/cmake/gtest.cmake)

option(GTEST_DISABLE_PTHREADS "" OFF)

option(QUEUE "" ON)

OPTION_TURN_ON_TESTING(QUEUE queue-utest queue_test.cpp check-queue
    "Тестирование p_queue отключено!\n")

