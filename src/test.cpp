#include <custom_stl_vector.h>
#include <gtest/gtest.h>

using namespace kirillidk_containers;

// Tests default constructor
TEST(VectorTest, DefaultConstructor) {
    vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

// Tests copy constructor
TEST(VectorTest, CopyConstructor) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);

    vector<int> v2(v1);
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 10);
    EXPECT_EQ(v2[1], 20);
}

// Tests push_back() function
TEST(VectorTest, PushBack) {
    vector<int> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

// Tests reserve() function
TEST(VectorTest, Reserve) {
    vector<int> v;
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
}

// Tests assignment operator
TEST(VectorTest, AssignmentOperator) {
    vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);

    vector<int> v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
}
