#include <iostream>

template <typename T>
auto add(T a, T b) -> int //int | double | decltype(a)
{
    return a + b;
}

int main()
{
    std::cout << add(10, 10) << std::endl; //20
    std::cout << add(10.7, 10.7) << std::endl; //21

    return 0;
}