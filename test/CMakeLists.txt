include(${PROJECT_SOURCE_DIR}/cmake/gtest.cmake)

function(OPTION_TURN_ON_TESTING 
    CONDITION TEST_NAME TEST_SOURCE EXEC_TARGET_NAME)
    if (NOT ${CONDITION})
        message("Тестирование класса OctalDigit отключено!\n")
    else()
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} 
            PRIVATE 
                Project::source
                GTest::GTest)
        add_custom_target(${EXEC_TARGET_NAME} ALL COMMAND ${TEST_NAME})
    endif()    

endfunction()

option(GTEST_DISABLE_PTHREADS "" OFF)

# option(SOME_MODULE_TESTING "" OFF)

# OPTION_TURN_ON_TESTING(SOME_MODULE_TESTING project-some_module-utest some_module.cpp check-some_module)

