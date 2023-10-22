#include <iostream>
#include <numeric>

template <typename First, typename... Args>
auto sum(const First first, const Args... args) -> decltype(first) 
{
  const auto values = {first, args...};
  return std::accumulate(values.begin(), values.end(), First {0});
}

int main()
{
    std::cout << sum(1, 2, 3, 4, 5) << std::endl; // 15
    std::cout << sum(1, 2, 3) << std::endl; // 6
    std::cout << sum(1.5, 2.0, 3.7) << std::endl;  // 7.2
}
