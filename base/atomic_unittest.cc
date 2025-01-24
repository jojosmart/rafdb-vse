// Author:Zhouxiaoliang

#include "base/atomic.h"
#include "base/thread_pool.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

TEST(AtomicCompareAndSwap, int32) {
  int32 a = 0;
  int32 b = 1;
  int32 c = 2;
  ASSERT_EQ(0, AtomicCompareAndSwap(&a, b, c));
  ASSERT_EQ(0, a);
  ASSERT_EQ(0, AtomicCompareAndSwap(&a, a, b));
  ASSERT_EQ(1, a);
}

TEST(AtomicCompareAndSwap, int64) {
  int64 a = 0;
  int64 b = 3;
  int64 c = 9;
  ASSERT_EQ(0, AtomicCompareAndSwap(&a, b, c));
  ASSERT_EQ(0, a);
  ASSERT_EQ(0, AtomicCompareAndSwap(&a, a, b));
  ASSERT_EQ(3, a);
}

class Foo {
 public:
  int foo;
};

TEST(AtomicPointerAssgin, SingleThread) {
  Foo f1;
  Foo f2;
  Foo *f = &f1;
  f1.foo = 0;
  f2.foo = 2;
  ASSERT_EQ(0, f->foo);
  AtomicPointerAssgin(&f, &f2);
  ASSERT_EQ(2, f->foo);
}

TEST(AtomicIncrement, int32) {
  int32 foo = 0;
  AtomicIncrement(&foo, 2);
  ASSERT_EQ(2, foo);
  AtomicIncrement(&foo, -22);
  ASSERT_EQ(-20, foo);
}

TEST(AtomicIncrement, int64) {
  int64 foo = 0;
  AtomicIncrement(&foo, static_cast<int64>(2));
  ASSERT_EQ(2, foo);
  AtomicIncrement(&foo, static_cast<int64>(-22));
  ASSERT_EQ(-20, foo);
}

void Increment(int64 *p, int k) {
  for (int i = 0; i < k; ++i)
    AtomicIncrement(p);
}

TEST(AtomicIncrement, MultiThread) {
  static int64 counter = 0;
  const int thread_num = 100;
  const int increment_num = 100;

  ThreadPool *pool = new ThreadPool(thread_num);
  for (int i = 0; i < thread_num; ++i)
    pool->Add(NewOneTimeCallback(Increment, &counter, increment_num));
  pool->StartWorkers();
  // wait for all threads' stop
  delete pool;
  ASSERT_EQ(thread_num * increment_num, counter);
}

}  // namespace base
