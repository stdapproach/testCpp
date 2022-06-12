#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <map>
#include <cassert>

using namespace std;

namespace cpp11{
template <class T>
void swap(T& a, T& b)
{
    static_assert(std::is_copy_constructible<T>::value,
                  "Swap requires copying");
    auto c = b;
    b = a;
    a = c;
}
struct no_copy
{
    no_copy(const no_copy&) = delete;
    no_copy() = default;
};

//template<class T, class U>
//auto add(T t, U u) { return t + u; }
//constexpr
    constexpr int f();
    constexpr bool b1 = noexcept(f()); // false, undefined constexpr function
    constexpr int f() { return 0; }
    constexpr bool b2 = noexcept(f()); // true, f() is a constant expression

    constexpr int factorial(int n)
    {
        return n <= 1 ? 1 : (n * factorial(n - 1));
    }

    template<int n>
    struct constN
    {
        constN() { std::cout << n << '\n'; }
    };

    //template aliases
    template<class T>
    using ptr = T*;
    //the name 'ptr<T>' is now an alias for pointer to T
    ptr<int> x;

    //default/delete
    struct type
    {
        type() = default; //trivial
        virtual ~type() = default; //non-trivial because virtual
        type & operator =( const type & ); // non-trivial because not defaulted here
    };
    inline type & type::operator =( const type & ) = default;

//nullptr
    template<class T>
    constexpr T clone(const T& t)
    {
        return t;
    }

    void g(int*)
    {
        std::cout << "Function g called\n";
    }
    //Trailing return type declaration
    template <class T, class U> auto add(T t, U u) -> decltype(t + u);

    //variadic template
    template<class ... Types> struct Tuple {};
    Tuple<> t0;           // Types contains no arguments
    Tuple<int> t1;        // Types contains one argument: int
    Tuple<int, float> t2; // Types contains two arguments: int and float
    //Tuple<0> error;     // error: 0 is not a type

    struct Foo {
        std::vector<int> mem = {1,2,3}; // list-initialization of a non-static member
        std::vector<int> mem2;
        Foo() : mem2{-1, -2, -3} {} // list-initialization of a member in constructor
    };

    struct S//Non-static data member initializers
    {
        int n = 7;
        std::string s{'a', 'b', 'c'};
        S() // default member initializer will copy-initialize n, list-initialize s
        { }
    };
}

namespace mem {
    int *pi;
}

namespace mov {//move-semantics
    struct A {
        int a1;
        string str;
        A(const char* pch):str(pch){cout <<"ctrA, str="<<str<<endl;}
        A(const A& rh):a1{rh.a1},str{rh.str} {cout <<"copy ctrA, str="<<str<<endl;};
        ~A(){cout <<"dtrA, str="<<str<<endl;}
        void boo() {cout <<"boo str="<<str<<"\n";}
        void boo()const {cout <<"boo const str="<<str<<"\n";}
    };
    void foo(A a) {cout<< "foo(A a), str="<<a.str<<"\n";}
    //void foo(const A a) {cout<< "foo(const A a)\n";}-redefinition
    void foo(A& a) {cout<< "foo(A& a), str="<<a.str<<"\n";}
}

int main()
{
    cout << "Hello C++11 World!" << endl;
    {//static_assert
        int a, b;
        swap(a, b);
        cpp11::no_copy nc_a, nc_b;
        //swap(nc_a, nc_b); // 1 //assert Swap requires copying
    }
    {//type_traits
        cout << std::boolalpha << endl;
        cout << std::is_floating_point<double>::value << endl;
        cout << std::is_array<int []>::value << endl;
    }
    {//auto
        auto (*p)() -> int;
        auto a = 1 + 2;
        auto c0 = a;
        decltype(a) c1 = a;
        auto d = {1, 2};
        auto n = {5};
        auto m{5};//initializer_list<int> in C++11
        auto lambda = [](int x) {return x + 3;};
    }
    {//delegating ctr
        class Foo {
        public:
          Foo(char x, int y) {}
          Foo(int y) : Foo('a', y) {} // Foo(int) delegates to Foo(char,int)
        };
    }

    static_assert(cpp11::factorial(5)==120,"5!==120");
    cpp11::constN<cpp11::factorial(4)> out1;
    volatile int k = 4; //disallow optimization using volatile
    cout << k << "! = " << cpp11::factorial(k) << '\n';// computed at runtime

    {//Strongly-typed
        enum class Color { red, green = 20, blue };
        Color r = Color::blue;
        switch(r)
        {
            case Color::red  : std::cout << "red\n";   break;
            case Color::green: std::cout << "green\n"; break;
            case Color::blue : std::cout << "blue\n";  break;
        }
        //int n1 = r;//error: no scoped enum to int conversion
        int n2 = static_cast<int>(r); // OK, n2 = 21
    }
    {//nullptr
        using namespace cpp11;//Inline namespaces
        g(nullptr);        // Fine
        g(NULL);           // Fine
        g(0);              // Fine

        g(clone(nullptr)); // Fine
        //g(clone(NULL));    // ERROR: non-literal zero cannot be a null pointer constant
        //g(clone(0));       // ERROR: non-literal zero cannot be a null pointer constant
    }
    {//List initialization
        const int (&ar)[2] = {1,2};// binds a lvalue reference to a temporary array
        int&& r1 = {1};// binds a rvalue reference to a temporary int
        //int bad{1.0}; // error: narrowing conversion
    }
    {
        std::string s1 = "Test";
        //std::string&& r1 = s1;           // error: can't bind to lvalue

        const std::string& r2 = s1 + s1; // okay: lvalue reference to const extends lifetime
        //r2 += "Test";                    // error: can't modify through reference to const

        std::string&& r3 = s1 + s1;      // okay: rvalue reference extends lifetime
        r3 += "Test";                    // okay: can modify through reference to non-const
        std::cout << r3 << '\n';
    }
    {
        std::vector<int> v{1,2,3,4,5};
        std::vector<int> v2(std::move(v)); // binds an rvalue reference to v
        assert(v.empty());
    }
    {//lambda
        auto glambda = [](int a, float&& b) { return a < b; };//no generix lambda in C++11
        bool b = glambda(3, 3.14); // ok
    }
    {//range for and structured binding declaration
        std::map<int, double> map;
        for (auto&& [first,second] : map) {
            // use first and second
        }
    }
    //override and final

    //2delete
    {
        using namespace mem;
        pi = new int(42);
        cout << "i=" << *pi << endl;
        delete pi;
        //cout << "2i=" << *pi << endl;
        pi = nullptr;
        //cout << "3i=" << *pi << endl;
        delete pi;
    }
    {//move semantic
        using namespace mov;
        //A a1("a1");
        const A a2{"a2"};
        //a1.boo();
        a2.boo();
    }

    return 0;
}
