// Author:Zhouxiaoliang

#include "base/shared_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

class SharedPtrTest : public testing::Test {
};

TEST_F(SharedPtrTest, Test1) {
  base::shared_ptr<int> s;
  ASSERT_EQ(s.use_count(), 1);
}
