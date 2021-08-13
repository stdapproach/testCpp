/*cpp20 examples, 
based on: https://www.youtube.com/watch?v=FRkJCvHWdwQ
*/

#include <vector>
#include <ranges>
#include <concepts>
#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include <span>
#include <map>
//#include <format>
//#include <experimental/generator> // MSVS's feature

#include "cpp20.h"

using namespace std;

namespace cpp20 {
namespace range{
    vector data {1,2,3,4,5,6,7,8,9,10};   
}

namespace concept_ {
    template<>
    void Foo(int i) {std::cout<< "Foo<int>, arg=" << i <<"\n";}

    //template<>
    //void Foo(std::string i) {} // no compiled with clear message
}

namespace lambda {
    //since C++20, you need explicitely add "this" to capture-clause
    //template lambda expression
    //[] <typename T> (T x){/**/}
    //[] <typename T> (T* px){/**/}
    //[] <typename T, int N> (T (&a)[N]){/**/}
    //example
    /*
    []<typename T>(const vector<T>& vec) {
        T x{  };
        T::static_function();
        //...
    };
    */
    //example2
    /*
    []<typename ...T>(T&& ...args) {
        return foo(std::forward<T>(args)...);
    };
    */
    //init-capture followed by an ellipsis is well-formed wince C++20
    /*
    template<class F, class ... Args>
    auto delay_invoke(F f, Args... args) {
        return [f=std::move(f), args = std::move(args)...] {
            return std::invoke(f, args...);
        };
    }
    */
}

namespace constexpr_{
    //constexpr auto v1 = vector<int>{1,2,3};
    class Foo {
        //constexpr auto get1() const{ return vector<int>{1,2,3};}
        constexpr ~Foo(){
            //allowed dynamic_cast, typeid
            //allowed new/delete
            //allowed try/catch
            // still NOT allowed to throw exceptions
        };
    };
}

namespace concurrency {
    //atomic<shared_ptr<T>> allowed
    // std::jthread: support cooperative cancelation, ask thread to cancel, calls join() destructor
    void DoWorkerPre20() {
        std::thread job{[]{/**/}};

        try {
            //
        } catch (...) {
            job.join();
            throw; //rethrow
        }
        job.join();//?check isJoinable
    }

    void doWork() {
        //std::jthread job{[]{/**/}};
        // ... Do something else ...
    }// jthread destructor automatically cancels thread and call join()

    //example cooperative cancelation
    void doStop() {
        // std::jthread job{ [](std::stop_token token) {
        //     while (!token.stop_requested()) {
        //         //...
        //     }
        // }};
        // job.request_stop();
    }
}

namespace templ {
    template<typename ValueType, ValueType DefaultValue>
    class SomeClass
    {
        public:
            SomeClass() : m_value(DefaultValue){}
            ValueType m_value;
    };
}

//immediate function
    consteval auto InchToMm(double inch) {return inch * 25.4;}
    consteval int sqr(int n) {
        return n*n;
    }

}

int main()
{
    cout << "Hello C++20 World!" << endl;
    {//range
        auto result{cpp20::range::data 
                    | views::filter([](const auto& val){return val % 2 == 0;}) // 2 4 6 8 10
                    | views::transform([](const auto& val){return val * val;}) // 4 16 36 64 100
                    | views::drop(2) // drop first 2 elements, 36 64 100
                    | views::reverse // 100 64 36
                    | views::transform([](const auto& it){return to_string(it);}) //"100" "64" "36"
        };
        for(auto it: result) {
            cout << "it=" << it << endl;
        }

        auto resultInf{views::iota(10)
         | std::views::take(2) // potencially an infinite range
        };
        for(auto it: resultInf) {
            cout << "it2=" << it << endl;
        }
    }
    {
        //using namespace cpp20::concept_;
        cpp20::concept_::Foo(42);
    }
    {//designated initialization of aggregates
        struct Data {
                int anInt{0};
                std::string aString;
                auto operator<=>(const Data&) const = default;//default spaceship operator
        };
        Data d {.aString = "HW"};
    }
    {
        //spaceship operator
        int i{42};
        strong_ordering result {i<=>0};
        if (result == strong_ordering::less) {cout << "less\n";}
        if (result == strong_ordering::greater) {cout << "greater\n";}
        if (result == strong_ordering::equal) {cout << "equal\n";}
        //other way to check
        if (is_lt(result)) {cout << "less\n";}
        if (is_gt(result)) {cout << "greater\n";}
        if (is_eq(result)) {cout << "equal\n";}
    }
    {
        //init for switch
        //C++17
        struct Foo {int value; int result; vector<int> values;};
        auto getData = [](){return Foo{};};
        switch(auto data{getData()}; data.value) {
             case 1:
                return 42;
        };
        //C++17
        if (auto data{getData()}; data.value == 42) {
            //...
        }
        //C++20 for range-based loop
        for(auto data{getData()}; auto& it: data.values) {
            cout << it;
        }
    }
    {
        //template
        using namespace cpp20::templ;
        SomeClass<int, 1> intObj; //ok
        SomeClass<float, 42.f> floatObj; //ok, since C++20
        cout << "\ndefVal=" << floatObj.m_value << "\n";
    }
    {
        //hints for compiler
        int val{2};
        switch (val) {
            case 1:
                break;
            [[likely]]case 2:
                //...
                break;
            [[unlikely]] case 3:
                break;
        }
    }
    {
        //date/time
        using namespace std::chrono;
        auto y1{year{2021}};
        auto y2 {2020y};
        auto m1{month{9}};
        auto mInvalid{month{13}};//well compiled
        auto m2{September};
        auto d1{day{15}};
        auto d2{15d};
        year_month_day fullDate{2021y, month{13}, 15d};//well compiled
        //std::chrono::zoned_time denver{"America/Denver", sys_days{2020y/September/15d}+9h};
        //auto localt{zoned_time{current_zone(), system_clock::now()}};
    }
    {//span
        int data[42];
        span<int, 42> a{data};//fixed size
        span<int> b{data};//dynamic-size
        //span<int, 50> a50{data};//compiled error
        //supportsd iterator (begin, cbegin etc)
        // front/back, operator[], data()
        // size(), size_bytes(), empty()
        // subspan(offset, count)
    }
    {
        constexpr double const_inch{6};
        const auto mm1{cpp20::InchToMm(const_inch)};//
        cout << mm1;
        //double dinch{8};
        //const auto mm2{cpp20::InchToMm(dinch)};//compilation error, due to dinch isn;t constexpr
        //
        constexpr auto res1 = cpp20::sqr(5);
        cout << res1;
        constinit static auto res2 = cpp20::sqr(5);//can only be applied to a variable with static or thread storage duration
        cout << res2;
    }
    {
        //test formating
        //cout << format("{=^19", "CppCon 2020"); // ====CppCon 2020====
        //cout << format("Read {0} bytes from {1}", n ,"file1.txt")
    }
    { //
        //const source_location& location = source_location::current();
        //cout << location.file_name() << location.line() << ":" << "QQQ" << endl;
    }
    {//bit operation
        //rotate rotl(), rotr()
        //countl_zero(), countl_one(), popcount()
    }
    {
        std::string str{"Hello World!"};
        bool b{str.starts_with("Hello")};
        cout << b << endl;
        //contains
        std::map myMap{std::pair{1, "one"s}, {2, "two"s}, {3, "three"s}};
        bool res{myMap.contains(2)};
        cout << res;
    }

}