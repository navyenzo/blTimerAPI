#ifndef BL_TIMERAPI_HPP
#define BL_TIMERAPI_HPP


//-------------------------------------------------------------------
// FILE:            blTimerAPI.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of data structures to facilitate
//                  the creation/use of timers running in parallel
//                  threads
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//-------------------------------------------------------------------



//-------------------------------------------------------------------

// A functor that wraps an object's member function so
// that it can easily be invoked by our timer

#include "blMemberFunctionWrapper.hpp"

// A timer that runs on a parallel thread and invokes
// multiple user provided callbacks every user's specified
// "period" seconds

#include "blTimer.hpp"

//-------------------------------------------------------------------


#endif // BL_TIMERAPI_HPP
