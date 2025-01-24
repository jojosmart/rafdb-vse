// Author:Zhouxiaoliang


#ifndef BASE_TIMER_H_
#define BASE_TIMER_H_

#include <string>
#include <sys/timerfd.h>

namespace base {
class Timer {
public:
    Timer()
            : closed_(false)
    {
        tfd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    }

    ~Timer()
    {
        ::close(tfd_);
    }

    bool After(int ms)
    {
        struct itimerspec new_value;
        new_value.it_interval.tv_nsec = 0;
        new_value.it_interval.tv_sec = 0;
        new_value.it_value.tv_nsec = static_cast<long>((ms % 1000) * 1000000);
        new_value.it_value.tv_sec = static_cast<time_t>(ms / 1000);
        int ret = ::timerfd_settime(tfd_, 0, &new_value, NULL);
        return ret==0 ? false : true;
    }

    
    int Fd() const
    {
        return tfd_;
    }

    bool IsReadable()
    {
        uint64_t one;
        return read(tfd_, &one, sizeof(one)) > 0;
    }

private:
    int tfd_;
    bool closed_;
};
}

#endif /* TIMER_H_ */
