cmake_minimum_required(VERSION 3.20)
project(my_project)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )

add_executable(
  tests 
  ${all_SRCS}
  unit_tests/tests.cpp
)

add_executable(
  rolling
  ${all_SRCS}
  main.cpp
)

target_include_directories(rolling PUBLIC include)
target_include_directories(tests PRIVATE include)

target_link_libraries(
  tests 
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
