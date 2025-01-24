// Author:Zhouxiaoliang

#include "base/mutex.h"
#include "base/thread_pool.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

void Read(int a, RwMutex *mu) {
  ReaderMutexLock lock(mu);
  LOG(INFO) << "a: " << a;
}

void Write(int *b, int value, RwMutex *mu) {
  WriterMutexLock lock(mu);
  *b = value;
}

TEST(RwMutexTest, Normal) {
  ThreadPool pool(100);
  int v = 0;
  RwMutex mu;
  for (int i = 0; i < 50; ++i)
    pool.Add(NewOneTimeCallback(Write, &v, i, &mu));
  pool.StartWorkers();
  for (int i = 0; i < 50; ++i)
    pool.Add(NewOneTimeCallback(Read, v, &mu));
}

}  // namespace base
