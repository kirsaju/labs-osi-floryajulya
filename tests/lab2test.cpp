#include <gtest/gtest.h>
#include "../lab2/include/medianFilter.h"
#include <chrono>

using namespace std::chrono;

TEST(SecondLabTests, SingleThreadIsCorrect) {
    std::vector<std::vector<double>> matrix = {
        {1, 2, 100},
        {2, 3, 4},
        {1, 5, -1000}
    };
    std::vector<std::vector<double>> expectedMatrix = {
        {2, 2.5, 3.5}, 
        {2, 2, 3.5},
        {2.5, 2.5, 3.5}
    };
    
    std::vector<std::vector<double>> result = applyMedianFilter(matrix, 3);

    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_DOUBLE_EQ(expectedMatrix[i][j], result[i][j]);
        }
    }
}

TEST(SecondLabTests, MultiThreadIsSameAsSingleThread) {
    vector<vector<double>> matrix(100, vector<double>(100));
    double value = 1.0;

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            matrix[i][j] = value;
            value += 0.1;
        }
    }
    
    std::vector<std::vector<double>> stResult = applyMedianFilter(matrix, 5);
    std::vector<std::vector<double>> mtResult = applyMedianFilterWithThreads(matrix, 5);

    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_DOUBLE_EQ(stResult[i][j], mtResult[i][j]);
        }
    }
}

TEST(SecondLabTests, MultiThreadIsFasterThanSingleThread) {
    vector<vector<double>> matrix(100, vector<double>(100));
    double value = 1.0;

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            matrix[i][j] = value;
            value += 0.1;
        }
    }
    auto start1 = high_resolution_clock::now();
    std::vector<std::vector<double>> stResult = applyMedianFilter(matrix, 11);
    auto end1 = high_resolution_clock::now();

    auto durationST = duration_cast<milliseconds>(end1 - start1);

    auto start2 = high_resolution_clock::now();
    std::vector<std::vector<double>> mtResult = applyMedianFilterWithThreads(matrix, 11);
    auto end2 = high_resolution_clock::now();

    auto durationMT = duration_cast<milliseconds>(end2 - start2);   

    ASSERT_GT(durationST.count(), durationMT.count());
}