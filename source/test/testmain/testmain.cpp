#include "testmain.h"

// using ::testing::Return;

MainTest::MainTest() {}

MainTest::~MainTest() {}

void MainTest::SetUp() {}

void MainTest::TearDown() {}

TEST_F(MainTest, TestZero) {
    my_int=0;
    EXPECT_EQ(0, my_int);
}

TEST_F(MainTest, TestOne) {
    my_int=1;
    ASSERT_EQ(1, my_int);
}