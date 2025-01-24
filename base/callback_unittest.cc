// Author:Zhouxiaoliang

#include "base/callback.h"
#include "base/yr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

class ClosureTest : public testing::Test {
 public:
  void SetA123Method()   { a_ = 123; }
  static void SetA123Function() { current_instance_->a_ = 123; }

  void SetAMethod(int a)         { a_ = a; }
  void SetCMethod(string c)      { c_ = c; }

  static void SetAFunction(int a)         { current_instance_->a_ = a; }
  static void SetCFunction(string c)      { current_instance_->c_ = c; }

  void SetABMethod(int a, const char* b)  { a_ = a; b_ = b; }
  static void SetABFunction(int a, const char* b) {
    current_instance_->a_ = a;
    current_instance_->b_ = b;
  }

  virtual void SetUp() {
    current_instance_ = this;
    a_ = 0;
    b_ = NULL;
    c_.clear();
  }

  int a_;
  const char* b_;
  string c_;

  static ClosureTest* current_instance_;
};

ClosureTest* ClosureTest::current_instance_ = NULL;

TEST_F(ClosureTest, TestClosureFunction0) {
  Closure* closure = NewOneTimeCallback(&SetA123Function);
  EXPECT_NE(123, a_);
  closure->Run();
  EXPECT_EQ(123, a_);
}

TEST_F(ClosureTest, TestClosureMethod0) {
  Closure* closure = NewOneTimeCallback(current_instance_,
                                        &ClosureTest::SetA123Method);
  EXPECT_NE(123, a_);
  closure->Run();
  EXPECT_EQ(123, a_);
}

TEST_F(ClosureTest, TestClosureFunction01) {
  Callback1<int>* callback = NewOneTimeCallback(&SetAFunction);
  EXPECT_NE(475, a_);
  callback->Run(475);
  EXPECT_EQ(475, a_);
}

TEST_F(ClosureTest, TestClosureFunction1) {
  Closure* closure = NewOneTimeCallback(&SetAFunction, 456);
  EXPECT_NE(456, a_);
  closure->Run();
  EXPECT_EQ(456, a_);
}

TEST_F(ClosureTest, TestClosureMethod01) {
  Callback1<int>* callback = NewOneTimeCallback(current_instance_,
                                                &ClosureTest::SetAMethod);
  EXPECT_NE(456, a_);
  callback->Run(456);
  EXPECT_EQ(456, a_);
}

TEST_F(ClosureTest, TestClosureMethod1) {
  Closure* closure = NewOneTimeCallback(current_instance_,
                                        &ClosureTest::SetAMethod, 456);
  EXPECT_NE(456, a_);
  closure->Run();
  EXPECT_EQ(456, a_);
}

TEST_F(ClosureTest, TestClosureFunction1String) {
  Closure* closure = NewOneTimeCallback(&SetCFunction, string("test"));
  EXPECT_NE("test", c_);
  closure->Run();
  EXPECT_EQ("test", c_);
}

TEST_F(ClosureTest, TestClosureMethod1String) {
  Closure* closure = NewOneTimeCallback(
      current_instance_, &ClosureTest::SetCMethod, string("test"));
  EXPECT_NE("test", c_);
  closure->Run();
  EXPECT_EQ("test", c_);
}

TEST_F(ClosureTest, TestClosureFunction02) {
  const char* cstr = "hello";
  Callback2<int, const char*>* callback = NewOneTimeCallback(&SetABFunction);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  callback->Run(789, cstr);
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
}

TEST_F(ClosureTest, TestClosureFunction12) {
  const char* cstr = "hello";
  Callback1<int>* callback = NewOneTimeCallback(&SetABFunction, cstr);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  callback->Run(789);
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
}

TEST_F(ClosureTest, TestClosureMethod2) {
  const char* cstr = "hello";
  Closure* closure = NewOneTimeCallback(current_instance_,
                                        &ClosureTest::SetABMethod, 789, cstr);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  closure->Run();
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
}

// Repeat all of the above with NewPermanentCallback()

TEST_F(ClosureTest, TestPermanentClosureFunction0) {
  Closure* closure = NewPermanentCallback(&SetA123Function);
  EXPECT_NE(123, a_);
  closure->Run();
  EXPECT_EQ(123, a_);
  a_ = 0;
  closure->Run();
  EXPECT_EQ(123, a_);
  delete closure;
}

TEST_F(ClosureTest, TestPermanentClosureMethod0) {
  Closure* closure = NewPermanentCallback(current_instance_,
                                          &ClosureTest::SetA123Method);
  EXPECT_NE(123, a_);
  closure->Run();
  EXPECT_EQ(123, a_);
  a_ = 0;
  closure->Run();
  EXPECT_EQ(123, a_);
  delete closure;
}

TEST_F(ClosureTest, TestPermanentClosureFunction1) {
  Closure* closure = NewPermanentCallback(&SetAFunction, 456);
  EXPECT_NE(456, a_);
  closure->Run();
  EXPECT_EQ(456, a_);
  a_ = 0;
  closure->Run();
  EXPECT_EQ(456, a_);
  delete closure;
}

TEST_F(ClosureTest, TestPermanentClosureMethod1) {
  Closure* closure = NewPermanentCallback(current_instance_,
                                          &ClosureTest::SetAMethod, 456);
  EXPECT_NE(456, a_);
  closure->Run();
  EXPECT_EQ(456, a_);
  a_ = 0;
  closure->Run();
  EXPECT_EQ(456, a_);
  delete closure;
}

TEST_F(ClosureTest, TestPermanentClosureFunction2) {
  const char* cstr = "hello";
  Closure* closure = NewPermanentCallback(&SetABFunction, 789, cstr);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  closure->Run();
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
  a_ = 0;
  b_ = NULL;
  closure->Run();
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
  delete closure;
}

TEST_F(ClosureTest, TestPermanentClosureMethod02) {
  const char* cstr = "hello";
  Callback2<int, const char*>* callback =
    NewPermanentCallback(current_instance_, &ClosureTest::SetABMethod);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  callback->Run(789, cstr);
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
  a_ = 0;
  b_ = NULL;
  callback->Run(799, cstr);
  EXPECT_EQ(799, a_);
  EXPECT_EQ(cstr, b_);
  delete callback;
}

TEST_F(ClosureTest, TestPermanentClosureMethod2) {
  const char* cstr = "hello";
  Closure* closure = NewPermanentCallback(current_instance_,
                                          &ClosureTest::SetABMethod, 789, cstr);
  EXPECT_NE(789, a_);
  EXPECT_NE(cstr, b_);
  closure->Run();
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
  a_ = 0;
  b_ = NULL;
  closure->Run();
  EXPECT_EQ(789, a_);
  EXPECT_EQ(cstr, b_);
  delete closure;
}

}  // anonymous namespace
