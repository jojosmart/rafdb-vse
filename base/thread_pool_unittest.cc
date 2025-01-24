// Author:Zhouxiaoliang

#include "base/string_util.h"
#include "base/thread.h"
#include "base/thread_pool.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

struct Task {
  bool done;
  Task() {
    done = false;
  }
};

class Producer : public Thread {
 public:
  explicit Producer(ProducerConsumerQueue<Task>* q, Task* t, int s, int l) {
    queue_ = q;
    tasks_ = t;
    start_ = s;
    length_ = l;
  }

 protected:
  virtual void Run() {
    for (int i = 0; i < length_; i++) {
      queue_->Put(&tasks_[start_+i]);
    }
  }

 private:
  ProducerConsumerQueue<Task>* queue_;
  Task* tasks_;
  int start_;
  int length_;
};

class Consumer : public Thread {
 public:
  explicit Consumer(ProducerConsumerQueue<Task>* q, int id) {
    queue_ = q;
    id_ = id;
  }

 protected:
  virtual void Run() {
    Task* task;
    while ((task = queue_->Get()) != NULL) {
      CHECK(!task->done);
      task->done = true;
      VLOG(1) << StringPrintf("Consumer %d consumes data ", id_);
    }
  }

 private:
  ProducerConsumerQueue<Task>* queue_;
  int id_;
};

TEST(PCQTest, LargeQueue) {
  ProducerConsumerQueue<int>* queue = new ProducerConsumerQueue<int>(INT_MAX);
  for (int i = 0; i < 99999; i++) {
    queue->Put(new int(i));
  }
  for (intptr_t i = 0; i < 99999; i++) {
    int *v = queue->Get();
    EXPECT_EQ(i, *v);
    delete v;
  }
  delete queue;
}

TEST(PCQTest, GetAndTryGet) {
  ProducerConsumerQueue<int>* queue = new ProducerConsumerQueue<int>(1);
  int i = 1;
  int* j = NULL;
  queue->Put(&i);
  EXPECT_TRUE(queue->TryGet(&j));
  EXPECT_EQ(*j, 1);
  EXPECT_FALSE(queue->TryGet(&j));
  delete queue;
}

TEST(PCQTest, PutAndTryPut) {
  ProducerConsumerQueue<int>* queue = new ProducerConsumerQueue<int>(1);
  int i = 1;
  EXPECT_TRUE(queue->TryPut(&i));
  EXPECT_FALSE(queue->TryPut(&i));
  EXPECT_EQ(*(queue->Get()), 1);
  delete queue;
}

TEST(PCQTest, Concurrency) {
  // Initialize
  const int num_producers = 10;
  const int num_consumer = 100;
  const int num_items = 10000;
  const int step = num_items / num_producers;
  Producer** producers = new Producer*[num_producers];
  Consumer** consumer = new Consumer*[num_consumer];
  Task* items = new Task[num_items];
  ProducerConsumerQueue<Task> queue(10);

  for (int i = 0; i < num_producers; i++) {
    producers[i] = new Producer(&queue, items, i*step, step);
    producers[i]->SetJoinable(true);
  }
  for (int i = 0; i < num_consumer; i++) {
    consumer[i] = new Consumer(&queue, i);
    consumer[i]->SetJoinable(true);
  }

  // Start
  for (int i = 0; i < num_producers; i++) {
    producers[i]->Start();
  }
  for (int i = 0; i < num_consumer; i++) {
    consumer[i]->Start();
  }

  // Join
  for (int i = 0; i < num_producers; i++) {
    producers[i]->Join();
  }
  for (int i = 0; i < num_consumer; i++) {
    queue.Put(NULL);  // Terminate consumers.
  }
  for (int i = 0; i < num_consumer; i++) {
    consumer[i]->Join();
  }

  // Check
  for (int i = 0; i < num_items; i++) {
    CHECK(items[i].done);
  }

  // Release
  for (int i = 0; i < num_producers; i++) {
    delete producers[i];
  }
  for (int i = 0; i < num_consumer; i++) {
    delete consumer[i];
  }
  delete[] producers;
  delete[] consumer;
  delete[] items;
}

}  // namespace base
