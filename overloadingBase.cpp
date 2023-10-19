#include <iostream>

class BaseClass
{
public:
    BaseClass() = default;
    ~BaseClass() = default;

    void send(std::string messeage) { std::cout << "BaseClass" << std::endl; }
};

class ChildClass : public BaseClass
{
public:
    ChildClass() = default;
    ~ChildClass() = default;

    //using BaseClass::send;
    void send(int messeage) { std::cout << "ChildClass" << std::endl; }
};

class SecondChildClass : public BaseClass
{
public:
    SecondChildClass() = default;
    ~SecondChildClass() = default;

    /*This makes the someFunction
       declaration in ParentClass visible
       here as well:
    */
    using BaseClass::send;
    
    void send(int messeage) { std::cout << "ChildClass" << std::endl; }
};


int main()
{
    ChildClass test;
    test.BaseClass::send(std::string("test"));

    SecondChildClass test2;
    test2.send(std::string("test"));
}