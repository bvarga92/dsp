#ifndef _SEMAPHORE_HPP_
#define _SEMAPHORE_HPP_

#include <mutex>
#include <condition_variable>
#include <chrono>

class Semaphore{
    private:
        std::mutex m;
        std::condition_variable cv;
        int cntr = 0;

    public:
        inline void notify(){
            std::lock_guard<std::mutex> lock(m);
            cntr++;
            cv.notify_one();
        }

        inline void wait(){
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&]{ return cntr > 0; });
            cntr--;
        }

        inline bool wait_for_ms(int millis){
            std::unique_lock<std::mutex> lock(m);
            bool finished = cv.wait_for(lock, std::chrono::milliseconds(millis), [&]{ return cntr > 0; });
            if(finished) cntr--;
            return finished;
        }
};

#endif
