#include <iostream>
#include <future>
#include <chrono>
#include <random>
#include <thread>
#include <exception>

using namespace std;
using ms = chrono::microseconds;

int doSth(char c){
    mt19937 dre(c);
    uniform_int_distribution<int> id(10,1000);

    for(int i = 0; i < 10; ++i){
        this_thread::sleep_for(ms(id(dre))); // 10 is too small, without sleep, it's impossible to show the concurrency. Try use 100000 without sleep.
        cout.put(c).flush();
    }
    return c;
}

int func1(){
    return doSth('.');
}

int func2(){
    return doSth('+');
}

int main(){
    cout << "starting func1 in background"
         << " and func2 in foreground:" << endl;

    // future<int> rst1(async(launch::deferred, func1));
    auto rst1(async(func1));
    // int rst1 = func1();
    // int rst2 = func2();
    auto rst2(async(func2));

    // int rst = rst1.get() + rst2;

    // cout << "\nresult of func1 + func2: " << rst << endl;
    if(rst1.wait_for(chrono::seconds(0)) != future_status::deferred
        || rst2.wait_for(chrono::seconds(0)) != future_status::deferred){
        while(rst1.wait_for(chrono::seconds(0)) != future_status::ready
        && rst2.wait_for(chrono::seconds(0)) != future_status::ready){
            this_thread::yield();
        }
    }
    cout.put('\n').flush();

    try{
        rst1.get();
        rst2.get();
    }
    catch(const exception& e){
        cout << "\nException: " << e.what() << endl;
    }
    cout << "\ndone." << endl;

    return 0;
}