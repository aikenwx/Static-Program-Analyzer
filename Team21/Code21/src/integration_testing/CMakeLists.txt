file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs} src/TestSuchThatClauses.cpp src/TestSelectClause.cpp src/QueryTestUtilities.h)

target_link_libraries(integration_testing spa)

include(CTest)
include(Catch)
catch_discover_tests(integration_testing)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
