// Copyright

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <functional>
#include <queue>
#include <sstream>

using namespace std;

template <typename T>
class ConcurrentQueue {
public:
    T pop() {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        auto val = queue_.front();
        queue_.pop();
        mlock.unlock();
        cond_.notify_one();
        return val;
    }

    void pop(T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        item = queue_.front();
        queue_.pop();
        mlock.unlock();
        cond_.notify_one();
    }

    void push(const T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.size() >= BUFFER_SIZE) {
            cond_.wait(mlock);
        }
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
    ConcurrentQueue() = default;
    ConcurrentQueue(const ConcurrentQueue&) = delete;            
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete; 

private:
    queue<T> queue_;
    mutex mutex_;
    condition_variable cond_;
    const static unsigned int BUFFER_SIZE = 10;
};


void produce(ConcurrentQueue<int>& q) {
    for (int i = 0; i < 100; ++i) {//#elements
        cout << "insertando: " << i << "\n";
        q.push(i);
    }
}

void consume(ConcurrentQueue<int>& q, unsigned int id) {
    for (int i = 0; i < 50; ++i) {
        auto item = q.pop();
        cout << "Consumer: " << id << " borro: " << item << "\n";
    }
}

int main(int argc, char* argv[]) {
    /*if (argc != 3) {
        return 1;
    }*/
    ConcurrentQueue<int> q;

    using namespace std::placeholders;

    // producer thread
    std::thread prod1(std::bind(produce, std::ref(q)));

    // consumer threads
    std::thread consumer1(std::bind(&consume, std::ref(q), 1));
    std::thread consumer2(std::bind(&consume, std::ref(q), 2));
    std::thread consumer3(std::bind(&consume, std::ref(q), 3));
    std::thread consumer4(std::bind(&consume, std::ref(q), 4));
    
    prod1.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();
   /* int number_producers = std::stoi(argv[1]);
    int number_consumers = std::stoi(argv[2]);

    ConcurrentQueue<std::string> queue;

    std::vector<std::thread*> producers;
    for (unsigned int i = 0; i < number_producers; ++i) {
        std::thread* producer_thread = new std::thread(produce(i, &queue));
        producers.push_back(producer_thread);
    }

    std::vector<std::thread*> consumers;
    for (unsigned int i = 0; i < number_consumers; ++i) {
        std::thread* consumer_thread = new std::thread(consume(i, &queue));
        consumers.push_back(consumer_thread);
    }
    */
    int stop;
    std::cin >> stop;
    return 0;
}