#include <iostream>
#include <mutex>
#include <list>
#include <ranges>
#include <algorithm>

bool flag { true };
std::list<int> container { 1, 2, 3, 4, 5 };
std::mutex mutex;

int main()
{
    if(auto it = std::find(container.begin(), container.end(), 2); it != container.end())
        std::cout << "Found" << std::endl;

    if (auto it = std::ranges::find(container, 2); it != container.end())
        std::cout << "Found" << std::endl;

    if(std::lock_guard lock(mutex); flag )
    {
        std::cout << "Unsetting flag" << std::endl;
        flag = false;
    }

    if (const auto keywords = {"if", "for", "while"};
        std::ranges::any_of(keywords, [keyword = "test"](const char* kw) { return keyword == kw; }))
    {
        std::cout << "Token must not be a keyword" << std::endl;;
    }
    else
    {
        std::cout << "Valid token" << std::endl;
    }
}