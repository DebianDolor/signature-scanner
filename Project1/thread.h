#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <future>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <memory>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>



class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        ->std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    std::vector< std::thread > workers;
    std::queue< std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};






#endif