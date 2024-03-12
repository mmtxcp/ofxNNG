#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class WaitGroup {
public:
    explicit WaitGroup(int count=1) : m_count(count) {}

    void add(int count) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_count += count;
    }

    void done() {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_count > 0) {
            m_count--;
            if (m_count == 0) {
                m_condition.notify_all();
            }
        }
    }

    void wait() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_count == 0; });
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    int m_count=1;
};

//int main() {
//    WaitGroup wg(5); // 指定初始数量为5
//
//    // 启动多个异步任务
//    for (int i = 0; i < 5; i++) {
//        wg.add(1);
//        std::thread([&wg, i]() {
//            // 模拟异步操作
//            std::this_thread::sleep_for(std::chrono::seconds(i));
//            std::cout << "Task " << i << " done" << std::endl;
//            wg.done();
//            }).detach();
//    }
//
//    // 等待所有异步任务完成
//    wg.wait();
//
//    std::cout << "All tasks completed" << std::endl;
//
//    return 0;
//}