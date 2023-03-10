file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs})

find_package(Catch2 REQUIRED)
target_link_libraries(integration_testing spa Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(integration_testing)
