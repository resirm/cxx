#include <condition_variable>
#include <mutex>
#include <future>
#include <thread>
#include <iostream>
#include <queue>

using namespace std;

queue<int> q;
mutex qMutex;
condition_variable qCondVar;

// provider_consumer

void provider(int val){
    for(int i = 0; i < 6; ++i){
        {
            lock_guard<mutex> lg(qMutex);
            q.push(val+i);
        }
        qCondVar.notify_one();
        this_thread::sleep_for(chrono::microseconds(val));
    }
}

void consumer(int num){
    while(true){
        int val;
        {
            unique_lock<mutex> ul(qMutex);
            qCondVar.wait(ul, []{ return !q.empty(); });
            val = q.front();
            q.pop();
        }
        cout << "consumer " << num << ": " << val << endl;
    }
}

int main(){
    auto p1 = async(launch::async, provider, 100);
    auto p2 = async(launch::async, provider, 300);
    auto p3 = async(launch::async, provider, 500);

    auto c1 = async(launch::async, consumer, 1);
    auto c2 = async(launch::async, consumer, 2);

    return 0;
}
