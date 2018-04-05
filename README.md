# blTimerAPI

[blTimerAPI](https://github.com/navyenzo/blTimerAPI.git) is a lightweight timer library based on std::thread that creates timers on parallel threads that fire at specified intervals for a specified amount of time

## Dependencies

* c++11
* std::thread

## How do I use it?

The blTimer class can be used with any type of function such as a function, a functor, a lambda function, with any number of arguments

The blTimerAPI also includes a blMemberFunctionWrapper object used to wrap member functions to use with the timer

Here's an example code where we feed a functor to our timer, and set the timer period to 1 second (that means it fires every second), set its total duration to 20 seconds (it stops firing after 20 seconds)

```c++
// Include the thread

#include <thread>

// Include iostream to output to console

#include <iostream>

// Include the blTimerAPI library

#include "blTimerAPI/blTimerAPI.hpp"



//-------------------------------------------------------------------
// A simple worker class
//-------------------------------------------------------------------
class WorkerClass
{
public:
    WorkerClass(){}
    ~WorkerClass(){}

    void workFunction(int& value)
    {
        std::cout << "value = " << ++value << std::endl;
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// main program function
//-------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Create the worker class

    WorkerClass myWorker;



    // Create the timer and set its
    // properties

    blTimerAPI::blTimer myTimer;

    myTimer.setDuration(5); // In seconds
    myTimer.setPeriod(1); // In seconds
    myTimer.setMaximumNumberOfTimesTheTimerFires(-1); // Negative means no max limit is set



    // The value variable

    int myValue = 0;



    // The member function wrapper

    auto functionWrapper = blTimerAPI::blMemberFunctionWrapper(myWorker,&WorkerClass::workFunction);



    // Start the timer

    myTimer.start(functionWrapper,std::ref(myValue));



    // Wait for the timer to complete
    //
    // NOTE: The timer could be stopped at
    //       any point by calling myTimer.stopAndDetach()
    //       or by calling myTimer.stopAndJoin()

    myTimer.join();



    // Print out the value after the thread
    // is done showing that the value was
    // indeed changed

    std::cout << "after thread is done, value = " << myValue << "\n\n\n";
    
    
    
    // We're done
    
    return 0;
}
//-------------------------------------------------------------------
```

The output from this program follows:

```c++
value = 1
value = 2
value = 3
value = 4
value = 5
after thread is done, value = 5
```