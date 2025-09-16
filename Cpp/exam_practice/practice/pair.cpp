#include <iostream>
#include <tuple>
#include <functional>

std::tuple<int, int> incr(int x, int y)
{
    return (std::make_tuple(++x, ++y));
}

int main(int argc, char **argv)
{
    // change by value and reference
    int n = 1;
    auto t = std::make_tuple(10, "Test", 3.142, std::ref(n), n);
    n = 7;

    std::cout << "The value of t is " << "("
              << std::get<0>(t) << ", " << std::get<1>(t) << "," << std::get<2>(t) << "," << std::get<3>(t) << ","
              << std::get<4>(t) << ")\n";

    // function returning multiple values
    int a = 1;
    int b = 1;
    std::tie(a, b) = incr(a, b);
    std::cout << a << " " << b << "\n";

    auto p = std::make_pair(1, "world!");
    std::cout << p.first << p.second << "\n";

    std::cout << std::get<0>(p) << " " << std::get<1>(p) << "\n";
}