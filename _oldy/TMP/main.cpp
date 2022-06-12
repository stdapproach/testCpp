#include <iostream>

using namespace std;

namespace sfinae {
/*
 * f(1, 2); -> overload resolution
 *  (1) void f(int, std::vector<int>);
    (2) void f(int, int);//the result of resolution
    (3) void f(double, double);
    (4) void f(int, int, char, std::string, std::vector<int>);
    (5) void f(std::string);
    (6) void f(...);
    (7) template<typename T>
        void f(T, T);
    (8) template<typename T>
        void f(T, typename T::iterator);
*/
template<typename T>
void f2(T x, typename T::type y)
{
    // ...
}

template<typename T>
void f2(T x, typename T::other_type y)
{
    // ...
}
}

struct St {
    St(){cout << "ctr St" << endl;}
    ~St(){cout << "dtr St" << endl;}
};

struct A {
    static St s;
};

//St A::s;

int main()
{
    cout << "Hello TMP World!" << endl;
    //St s;
    //A a;
    return 0;
}
