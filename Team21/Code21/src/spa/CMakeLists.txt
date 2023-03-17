file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/query_evaluators/such_that_evaluators/cfg_evaluators/cfg_evaluator.cpp src/query_evaluators/such_that_evaluators/cfg_evaluators/cfg_evaluator.h src/query_evaluators/such_that_evaluators/such_that_evaluator.cpp src/query_evaluators/such_that_evaluators/such_that_evaluator.h src/query_evaluators/such_that_evaluators/cfg_evaluators/next_evaluator.cpp src/query_evaluators/such_that_evaluators/cfg_evaluators/next_evaluator.h src/query_evaluators/such_that_evaluators/cfg_evaluators/next_star_evaluator.cpp src/query_evaluators/such_that_evaluators/cfg_evaluators/next_star_evaluator.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
