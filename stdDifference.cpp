#include <iostream>
#include <algorithm>
#include <list>

enum class test
{
    first,
    second,
    third,
    fourth,
    fifth,
    sixth
};

int main()
{
    std::list<test> list1 = { test::first, test::second, test::third, test::fourth, test::fifth, test::sixth };
    std::list<test> list2 = { test::second, test::first, test::sixth };
    list1.sort([](const test a, const test b){ return static_cast<int>(a) < static_cast<int>(b); });
    list2.sort([](const test a, const test b){ return static_cast<int>(a) < static_cast<int>(b); });

    std::list<test> diff;

    std::set_difference(list1.begin(), list1.end(),
                        list2.begin(), list2.end(),
                        std::back_inserter(diff),
                        [](auto& a, auto& b) { return static_cast<int>(a) < static_cast<int>(b); });
    
    std::cout << "list1" << std::endl;
    for(const auto& i : list1)
        std::cout << static_cast<int>(i);

    std::cout << std::endl << "list2" << std::endl;
    for(const auto& i : list2)
        std::cout << static_cast<int>(i);

    std::cout << std::endl << "diff" << std::endl;
    for(const auto& i : diff)
        std::cout << static_cast<int>(i);

}