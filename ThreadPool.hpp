//
// Created by fab on 12/10/2020.
//

#ifndef RAYTRACINGONEWEEKEND_THREADPOOL_HPP
#define RAYTRACINGONEWEEKEND_THREADPOOL_HPP


#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <queue>
#include <glm/vec3.hpp>
#include "Camera.hpp"

class ThreadPool
{
private:
    unsigned int threadNum;
    std::vector<std::thread> threads;

    //maybe replace that with a thread safe queue
    std::mutex queueMutex;

    using func = std::function<void()>;

    std::queue<func> jobQueue;

    bool working;

    void threadLoop()
    {
        while (working)
        {
            func f = nullptr;
            {
                std::unique_lock lock(queueMutex);

                if(!jobQueue.empty())
                {
                    f = jobQueue.front();
                    jobQueue.pop();
                }
            }

            if(f != nullptr)
                f();
        }
    }
public:
    ThreadPool()
    {
        working = true;
        threadNum = std::thread::hardware_concurrency();

        threads.reserve(threadNum);

        for (int i = 0; i < threadNum; ++i)
        {
            threads.emplace_back(&ThreadPool::threadLoop, this);
        }
    }

    ~ThreadPool()
    {
        working = false;

        for (int i = 0; i < threadNum; ++i)
        {
            threads[i].join();
        }
    }

    void addJob(func f)
    {
        jobQueue.push(f);
    }
};


#endif //RAYTRACINGONEWEEKEND_THREADPOOL_HPP
