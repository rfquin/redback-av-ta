cmake_minimum_required(VERSION 3.8)
project(nmp25)

# Google moment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -lpthread)
endif()

foreach(IDX RANGE 1 3 1)
	add_executable(Question-${IDX}-out Question-${IDX}/Question-${IDX}.cc)
	target_link_libraries(Question-${IDX}-out pthread)
	add_test(question-${IDX} Question-${IDX}-out)
endforeach()


# Question 4: Extension Task. You need to integrate Google Tests here
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip
)

# Microsoft moment
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Question-4-out Question-4/Question-4.cc)

target_link_libraries(
  Question-4-out
  GTest::gtest_main
  GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(Question-4-out)
