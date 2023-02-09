#include <iostream>
#include <optional>


std::optional<int> test(bool returnValue)
{
    if(returnValue)
        return 5;
    return std::nullopt;
}

int main()
{
    test(true);
    std::optional<int> result = test(true);
    if(result)
        std::cout << "Returned value is equal to: " << *result << std::endl;
    else
        std::cout << "Returned value is equal to nullopt" << std::endl;

    test(false);
    std::optional<int> result2 = test(false);
    if(result2)
        std::cout << "Returned value is equal to: " << *result2 << std::endl;
    else
        std::cout << "Returned value is equal to nullopt" << std::endl;
}