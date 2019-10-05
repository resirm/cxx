#include <iostream>
#include "memory_control.hpp"

using namespace std;

int main(){
    const int blk = 128, total = 2048;
    auto memPool = new MemCtl<blk, total>();
    int* test[32];
    int cnt = blk/sizeof(int); 
    for(int i = 0; i < 32; ++i){
        test[i] = static_cast<int*>(memPool->malloc());
        for(int j = 0; j < cnt; ++j){
            test[i][j] = j;
        }
    }
    for(int i = 0; i < 32; ++i){
        cout << "test[" << i << "]" << endl;
        for(int j = 0; j < cnt; ++j){
            cout << test[i][j] << ", ";
        }
        cout << endl;
    }
    memPool->info();
    int* a = static_cast<int*>(memPool->malloc());
    int* b = static_cast<int*>(memPool->malloc());
    // cnt+=1; //test out-of-range
    memPool->info();
    for(int i = 0; i < cnt; ++i){
        a[i] = i;
    }
    cout << "a: " << endl;
    for(int i = 0; i < cnt; ++i){
        cout << a[i] << ", ";
    }
    cout << endl;
    cout << "b: " << endl;
    for(int i = 0; i < cnt; ++i){
        cout << b[i] << ", ";
    }
    cout << endl;

    cout << "free a, b" << endl;
    
    memPool->free(static_cast<void*>(a));
    memPool->free(static_cast<void*>(b));

    cout << "realloc a, b" << endl;
    a = static_cast<int*>(memPool->malloc());
    b = static_cast<int*>(memPool->malloc());
    cout << "a: " << endl;
    for(int i = 0; i < cnt; ++i){
        cout << a[i] << ", ";
    }
    cout << endl;
    cout << "b: " << endl;
    for(int i = 0; i < cnt; ++i){
        cout << b[i] << ", ";
    }
    cout << endl;
    delete memPool;

    return 0;
}