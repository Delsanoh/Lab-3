

#include "gtest/gtest.h"
#include "../Лаба_3/functions.h"

TEST(MarkerLogicTest, MarksEmptyElement) {
    std::vector<int> arr = { 0, 0, 0 };
    bool result = try_mark_element(arr, 1, 42);
    EXPECT_TRUE(result);
    EXPECT_EQ(arr[1], 42);
}

TEST(MarkerLogicTest, CannotMarkOccupiedElement) {
    std::vector<int> arr = { 0, 7, 0 };
    bool result = try_mark_element(arr, 1, 42);
    EXPECT_FALSE(result);
    EXPECT_EQ(arr[1], 7);
}

TEST(MarkerLogicTest, CountsMarkedElements) {
    std::vector<int> arr = { 5, 0, 5, 3, 5 };
    EXPECT_EQ(count_marked_elements(arr, 5), 3);
    EXPECT_EQ(count_marked_elements(arr, 1), 0);
    EXPECT_EQ(count_marked_elements(arr, 3), 1);
}

TEST(MarkerLogicTest, ResetMarkedElements) {
    std::vector<int> arr = { 5, 7, 5, 3, 5 };
    reset_marked_elements(arr, 5);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 7);
    EXPECT_EQ(arr[2], 0);
    EXPECT_EQ(arr[3], 3);
    EXPECT_EQ(arr[4], 0);
}

TEST(MarkerLogicTest, BoundaryIndices) {
    std::vector<int> arr = { 0, 0, 0, 0 };
    EXPECT_TRUE(try_mark_element(arr, 0, 1));
    EXPECT_TRUE(try_mark_element(arr, 3, 2));
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[3], 2);
}