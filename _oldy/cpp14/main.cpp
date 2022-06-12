#include <iostream>
#include <memory>

using namespace std;

namespace cpp14 {
template<class T, class U>
decltype (auto) add(T t, U u) { return t + u; }

template<class T>
constexpr T pi = T(3.1415926535897932385L);  // variable template

template<class T>
T circular_area(T r) //function template
{
    return pi<T> * r * r; //pi<T> is a variable template instantiation
}

}

int main()
{
    cout << "Hello C++14 World!" << endl;
    cout << cpp14::add(7, 3.14) << endl;

    {
        //Generalized lambda captures
        //go.run( [ u=move(u) ] { do_something_with( u ); } ); // move the unique_ptr into the lambda
        int x = 4;
        int z = [&r = x, y = x+1] {
                    r += 2;         //set x to 6; "R is for Renamed Ref"
                    return y+2;     //return 7 to initialize z
                }(); // invoke lambda
    }
    {
        // generic lambda, operator() is a template with two parameters
        auto glambda = [](auto a, auto&& b) {return a < b;};
        bool b = glambda(3, 3.14); // ok

        // generic lambda, operator() is a template with one parameter
        auto vglambda = [](auto printer) {
            return [=](auto&&... ts) // generic lambda, ts is a parameter pack
            {
                printer(std::forward<decltype(ts)>(ts)...);
                return [=] { printer(ts...); }; // nullary lambda (takes no parameters)
            };
        };
        auto p = vglambda([](auto v1, auto v2, auto v3) { std::cout << v1 << v2 << v3; });
        auto q = p(1, 'a', 3.14); // outputs 1a3.14
        q();                      // outputs 1a3.14
    }
    {
        auto u = std::make_unique<const char*>("www");
    }
    {
        auto p = std::make_pair(1, 3.14);
        std::cout << '(' << std::get<0>(p) << ", " << std::get<1>(p) << ")\n";
        std::cout << '(' << std::get<int>(p) << ", " << std::get<double>(p) << ")\n";
    }
    return 0;
}
