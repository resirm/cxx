#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

long data;
atomic<bool> readyFlag(false);

void provider(){
    cout << "<return>" << endl;
    cin.get();

    data = 42;

    readyFlag.store(true);
}

void consumer(){
    while(!readyFlag.load()){
        cout.put('.').flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "\nvalue: " << data << endl;
}

int main(){
    auto p = async(launch::async, provider);
    auto c = async(launch::async, consumer);
}