// monadicclutter.cpp : Defines the entry point for the console application.
//

#include <cassert>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <iostream>

struct Foo { std::string bar; };
std::ostream &operator<<(std::ostream& out, const Foo &){ return out << "a Foo";}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main(){
    using Var = std::variant<int, double, Foo>;
    Var u;
    u = 5;


    if(0 == u.index()) { std::cout << std::get<0>(u) << " int\n"; }
    std::get<int>(u);
    if (auto p = std::get_if<int>(&u)){ std::cout << *p << " *int\n"; }

    std::visit([](auto x){ std::cout << x << "\n";}, u);
    struct V {
        void operator() (int){ std::cout << "int\n"; };
        void operator() (double){ std::cout << "double\n"; };
        void operator() (Foo){ std::cout << "Foo\n"; };
    } visitor;

    std::visit(visitor, u);

    std::visit([](auto u){
        if constexpr(std::is_same<int, decltype(u)>::value){ std::cout << "int!\n";}
    }, u);

    // the overload trick...
    std::visit(overloaded{
        [](int i){ std::cout << "INT!!!\n"; },
        [](double d){ std::cout << "DOUBLE!!!\n";},
        [](Foo foo){ std::cout << "FOO!!!\n";}
        }, u);
}