#include <iostream>
#include <tuple>
#include <vector>

int main()
{

    // make a vector of tuple
    std::vector<std::tuple<int, std::string, double>> people{
        std::make_tuple(2, "Brain", 101.1),
        std::make_tuple(3, "Beth", 202.2),
        std::make_tuple(4, "Kevin", 303.3)};

    for (const auto &element : people)
    {
        std::cout << std::get<0>(element) << "::"
                  << std::get<1>(element) << "::"
                  << std::get<2>(element) << std::endl;
    }

    // make a tuple
    std::tuple<int, std::string, double> entry{1, "kate", 100.0};

    // function returning multiple values
}
