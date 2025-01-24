// Author:Zhouxiaoliang

#include "base/concurrent_queue.h"
#include <unistd.h>
#include <vector>
#include "base/logging.h"
#include "base/thread.h"
#include "testing/gtest/include/gtest/gtest.h"

base::ConcurrentQueue<int> g_que;
bool g_stop = false;

class GetThread : public base::Thread {
 public:
  std::vector<int> vec_;
 protected:
  void Run() {
    for (int i = 0; i < vec_.size(); ++i) {
      int p;
      g_que.Pop(p);
      LOG(INFO) << "GET: " << p;
      EXPECT_EQ(vec_[i], p);
    }
  }
};

TEST(ConcurrentQueue, Normal) {
  GetThread thread;

  std::vector<int> vec;
  vec.push_back(3);
  vec.push_back(5);
  vec.push_back(1);
  thread.vec_ = vec;

  thread.Start();
  for (std::vector<int>::iterator itr = vec.begin(); itr != vec.end(); ++itr) {
    g_que.Push(*itr);
    sleep(1);
  }
}
