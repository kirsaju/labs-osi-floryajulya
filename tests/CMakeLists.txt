include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(lab2test lab2test.cpp ../lab2/include/medianFilter.h ../lab2/src/medianFilter.cpp)

target_link_libraries(lab2test PRIVATE GTest::GTest)
target_include_directories(lab2test PRIVATE ../lab2/include)

add_test(lab2test lab2test)

set(CMAKE_CXX_STANDARD 17)
