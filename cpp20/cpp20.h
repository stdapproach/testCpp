//
#include <iostream>

namespace cpp20 {
namespace concept_ {
    template<typename T>
    concept Incrementable = requires(T x){x++; ++x;};

    template<Incrementable T>
    void Foo(T t);

    template<typename T> requires Incrementable<T>
    void Foo2(T t);

    template<typename T>
    void Foo3(T t) requires Incrementable<T>;

    void Foo4(Incrementable auto t);

    template<typename T>
    concept C = requires (T& x, T& y) {
    	{x.swap(y)} noexcept; //requires a non-throwing swap() method
    	{x.size()} -> std::convertible_to<std::size_t>; // requires a size() method returning a size_t
    };
}
}