#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string rev(string& s){
    auto b = s.find_last_of('(');
    // string e;
    if(b == string::npos){
        if(s.find_first_of(')') == string::npos){
            return s;
        }
        else{
            return "";
        }
    }
    auto e = s.find_first_of(')', b);
    while(b != string::npos && e != string::npos){
        auto t = s.substr(b+1,e-b-1);
        reverse(t.begin(), t.end());
        s.replace(b, e-b+1, t);
        // cout << s << endl;
        b = s.find_last_of('(');
        e = s.find_first_of(')');
    }
    if(!(b == string::npos && e == string::npos))
        return "";
    return s;
}

int main(){
    string s = "((lw)av)deq(fs)e";
    // string s = "(((ab)cd)ef)";
    // string s = "uy)";
    string rst = rev(s);
    cout << rst << endl;
    return 0;
}