#include <iostream>
#include "fixed_thread_pool.hpp"
#include <chrono>
#include <string>

using namespace std;

void work(int num){
    // string rst = "work: " + to_string(num) + "\n";
    // cout << rst << flush;
    int rst = 0;
    for(int i = 0; i < num; ++i){
        rst += i;
    }
}

int main(){
    cout << "enter tasks num: " << flush;
    int num;
    cin >> num;
    using clk = chrono::steady_clock;
    
    while(!cin){
        cerr << "enter a number!" << endl;
        cin >> num;
    }
    cout << "\nstart..." << endl;
    clk::time_point begin = clk::now();
    clk::time_point end = clk::now();
    {
        fixed_thread_pool pool(100);
        begin = clk::now();
        for(int i = 0; i < num; ++i){
            pool.add(work, 1000);
        }
        while(pool.working()){
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        cout << "thread pool empty, quit." << endl;
        end = clk::now();
    }
    auto dur = chrono::duration_cast<chrono::milliseconds>(end-begin);
    
    
    begin = clk::now();
    for(int i = 0; i < num; ++i){
        work(i);
    }
    end = clk::now();
    auto dur2 = chrono::duration_cast<chrono::milliseconds>(end-begin);
    cout << "compare running time.\nmultithread: " << dur.count() << endl;
    cout << "synchronized: " << dur2.count() << endl;
    return 0;
}