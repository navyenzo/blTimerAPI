# blTimerAPI

[blTimerAPI](https://github.com/navyenzo/blTimerAPI.git) is a lightweight timer library based on std::thread that creates timers on parallel threads that fire at specified intervals for a specified amount of time

## Dependencies

* c++11
* std::thread

## How do I use it?

The blTimer class can be used with any type of function such as a function, a functor, a lambda function, with any number of arguments

Here's an example code where we feed a functor to our timer, and set the timer period to 1 second (that means it fires every second), set its total duration to 20 seconds (it stops firing after 20 seconds)

```c++
// Include the thread

#include <thread>

// Include iostream to output to console

#include <iostream>

// Include the blTimerAPI library

#include "blTimerAPI/blTimerAPI.hpp"



//-------------------------------------------------------------------
// A simple counting functor
//-------------------------------------------------------------------
class counter
{
public:

    counter(){}
    ~counter(){}

    void operator()(int& count)
    {
        std::cout << "count = " << ++count << std::endl;
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Create an instance of the counter
	
	counter myCounter;
	
	// The count
	
	int count = 10;
	
	// Create the timer
	
	blTimerAPI::blTimer myTimer;
	
	// Set the timer duration to 20 seconds total (stop after 20 seconds)
	
    myTimer.setDuration(20);
	
	// Set the period to 1 second (fire every 1 second)

    myTimer.start(std::ref(myCounter),std::ref(count));
	
	// Output the final count to
	// prove that the count was
	// changed
	
	std::cout << "Final count = " << count << std::endl;
	
	// We're done
	
	return 0;
}
//-------------------------------------------------------------------
```