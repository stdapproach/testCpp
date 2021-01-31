#include <iostream>
#include <utility>
#include <tuple>
#include <mutex>
#include <vector>
#include <algorithm>
#include <atomic>
#include <string_view>
#include <any>
#include <variant>

using namespace std;

namespace cpp17 {

namespace Q {
  namespace V { // original-namespace-definition for V
    void f(); // declaration of Q::V::f
  }
  void V::f() {} // OK
  //void V::g() {} // Error: g() is not yet a member of V
  namespace V { // extension-namespace-definition for V
    void g(); // declaration of Q::V::g
  }
}
namespace R { // not a enclosing namespace for Q
   //void Q::V::g() {} // Error: cannot define Q::V::g inside R
}
void Q::V::g() {} // OK: global namespace encloses Q

//fold expression
template<typename... Args>
bool all(Args... args) {return (... && args);}
bool b = all(true, true, true, false);
 // within all(), the unary left fold expands as
 //  return ((true && true) && true) && false;
 // b is false

template<class T> struct A { A(T,T); };
//auto y = new A{1,2}; // allocated type is A<int>

template <typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>)
        return *t; // deduces return type to int for T = int*
    else
        return t;  // deduces return type to int for T = int
}

// variable with external linkage included in multiple source files must be inline
inline std::atomic<int> counter(0);
}

int main()
{
    cout << "Hello C++17 World!" << endl;
    {//Class template argument deduction
        std::pair p(2, 4.5);     // deduces to std::pair<int, double> p(2, 4.5);
        std::tuple t(4, 3, 2.5); // same as auto t = std::make_tuple(4, 3, 2.5);
        std::less l;             // same as std::less<void> l;

        mutex mtx;
        auto lck = std::lock_guard(mtx); // deduces to std::lock_guard<std::mutex>

    }
    {//Copy elision
//        T f() {
//            return T();
//        }

//        f(); // only one call to default constructor of T
        //T x = T(T(f())); // only one call to default constructor of T, to initialize x
    }
    {
//        cout << i << i++; // undefined behavior until C++17
//        a[i] = i++;       // undefined behavior until C++17
//        n = ++i + i;      // undefined behavior
    }
    {//Structured binding
        int a[2] = {1,2};

        auto [x,y] = a; // creates e[2], copies a into e, then x refers to e[0], y refers to e[1]
        auto& [xr, yr] = a; // xr refers to a[0], yr refers to a[1]
    }
    {
        switch(int n = 1)
                case 0:
                case 1: std::cout << n << '\n';
    }
    {
        //std::size(), std::empty() and std::data();
    }
    {
        std::string_view str{ "Trains are fast!" };

        std::cout << str.length() << '\n'; // 16
        std::cout << str.substr(0, str.find(' ')) << '\n'; // Trains
        std::cout << (str == "Trains are fast!") << '\n'; // 1

          // Начиная с C++20
//          std::cout << str.starts_with("Boats") << '\n'; // 0
//          std::cout << str.ends_with("fast!") << '\n'; // 1
        std::cout << str << '\n'; // Trains are fast!
    }
    {
        // any type
        std::any a = 1;
        std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
        a = 3.14;
        std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
        a = true;
        std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';
    }//optional
    {
        std::variant<int, float> v, w;
        v = 12; // v contains int
        int i = std::get<int>(v);
        w = std::get<int>(v);
        w = std::get<0>(v); // same effect as the previous line
        w = v; // same effect as the previous line

        //  std::get<double>(v); // error: no double in [int, float]
        //  std::get<3>(v);      // error: valid index values are 0 and 1

        try {
          std::get<float>(w); // w contains int, not float: will throw
        }
        catch (const std::bad_variant_access&) {}
    }

    return 0;
}
