#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <cstdint>
#include <chrono>

enum Status 
{
    none = ( 0 << 0 ),
    newData = (1 << 1),
    newRequest = ( 1 << 2 )
};

std::mutex mutex;
std::atomic<int> status = Status::none;
std::atomic<bool> newDataSetterFinished = false;
std::atomic<bool> newRequestSetterFinished = false;

void NewDataSetter()
{
    for(int i = 0; i < 10'000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));

        //mutex.lock();
        status |= Status::newData; 
        //mutex.unlock();
    }

    newDataSetterFinished = true;
}

void NewRequestSetter()
{
    for(int i = 0; i < 10'000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));

        //mutex.lock();
        status |= Status::newRequest;
        //mutex.unlock();
    }

    newRequestSetterFinished = true;
}

void atomicTest()
{
    std::thread newDataSetter(&NewDataSetter);
    std::thread newRequestSetter(&NewRequestSetter);

    int receivedNewData = 0;
    int receivedNewRequest = 0;
    int commonProcessing = 0;
    int breakLoop = false;

    auto start = std::chrono::high_resolution_clock::now();

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //mutex.lock();
        int tmpStatus = status.load();

        while (status.compare_exchange_weak(tmpStatus, tmpStatus & (~(Status::newData | Status::newRequest))) == false);
        //mutex.unlock();
        if(tmpStatus & Status::newData)
            receivedNewData++;

        if(tmpStatus & Status::newRequest)
            receivedNewRequest++;

        if(tmpStatus & Status::newData && tmpStatus & Status::newRequest)
            commonProcessing++;

        if(breakLoop)
            break;

        if(newDataSetterFinished && newRequestSetterFinished)
            breakLoop = true;
    }

    newDataSetter.join();
    newRequestSetter.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    std::cout << "receivedNewData: " << receivedNewData << std::endl;
    std::cout << "receivedNewRequest: " << receivedNewRequest << std::endl;
    std::cout << "commonProcessing: " << commonProcessing << std::endl;
}

int main()
{
    atomicTest();

    return 0;
}

//receivedNewData: 10000
//receivedNewRequest: 10000
//commonProcessing: 6258