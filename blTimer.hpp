#ifndef BL_TIMER_HPP
#define BL_TIMER_HPP


//-------------------------------------------------------------------
// FILE:            blTimer.hpp
// CLASS:           blTimer
// BASE CLASS:      None
//
// PURPOSE:         A timer that runs on a parallel thread and invokes
//                  multiple user provided callbacks every user's specified
//                  period seconds
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
#include <thread>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class blTimer
{
public:

    // Default constructor

    blTimer();



    // Destructor

    virtual ~blTimer();



    // Eliminate copy construction

    blTimer(blTimer const&) = delete;

    blTimer&                                            operator=(blTimer const&) = delete;



public:  // Public functions



    // Function used to specify the timer period

    void                                                setPeriod(const int& periodExpressedInSeconds);



    // Function used to specify the timer's
    // total duration

    void                                                setDuration(const double& timerDurationInSeconds);



    // Functions used to stop the timer

    void                                                stopAndJoin();
    void                                                stopAndDetach();



    // pause function will signal the thread to pause
    // its execution to later be restarted again

    void                                                pause();



    // Functions used ot start the parallel thread

    template<typename blFunctor,
             typename...blFunctorArguments>

    bool                                                start(blFunctor functor);

    template<typename blFunctor,
             typename...blFunctorArguments>

    bool                                                start(blFunctor functor,
                                                              blFunctorArguments... arguments);



private:  // Private functions



    // run functions will call the registered
    // callbacks as the thread is being executed
    // this function takes as an argument a functor
    // with functor arguments and will call that
    // functor with passed arguments repeatedly
    // every time the timer hits

    template<typename blFunctor,
             typename...blFunctorArguments>

    void                                                run(blFunctor functor,
                                                            blFunctorArguments... arguments);



private:  // Private variables



    // The atomic variable used to signal
    // the thread to stop its execution

    std::atomic<bool>                                   m_stop;



    // The atomic variable used to "pause"
    // the thread, meaning that if the
    // pause variable is true, the thread
    // does not execute its function

    std::atomic<bool>                                   m_pause;



    // The timer period in seconds

    std::chrono::duration<double>                       m_period;



    // The total duration of the timer
    // after which the timer will be stopped

    std::chrono::duration<double>                       m_duration;



    // Time point at which timer was started

    std::chrono::high_resolution_clock::time_point      m_timeStarted;



    // The variable used to spawn parallel
    // therads

    std::thread                                         m_thread;

};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline blTimer::blTimer() : m_stop(true),m_pause(false),m_period(std::chrono::duration< double,std::ratio<1,1> >(1)),m_duration(std::chrono::duration<double>::zero())
{
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline blTimer::~blTimer()
{
    stopAndJoin();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void blTimer::setPeriod(const int& periodExpressedInSeconds)
{
    m_period = std::chrono::seconds(periodExpressedInSeconds);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void blTimer::setDuration(const double& timerDurationInSeconds)
{
    m_duration = std::chrono::duration< double,std::ratio<1,1> >(timerDurationInSeconds);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void blTimer::stopAndJoin()
{
    m_stop = true;

    if(m_thread.joinable() == true)
        m_thread.join();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void blTimer::stopAndDetach()
{
    m_stop = true;

    if(m_thread.joinable() == true)
        m_thread.detach();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void blTimer::pause()
{
    m_pause = true;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blFunctor,
         typename...blFunctorArguments>

inline bool blTimer::start(blFunctor functor)
{
    // We want this timer to only spin off
    // one thread, so we first check if the
    // timer had already started

    if(m_stop == true && m_thread.joinable() == false)
    {
        // This means the timer had been
        // stopped or not started yet, and
        // that the previous timer thread
        // has been detached or joined

        m_stop = false;
        m_pause = false;

        m_thread = std::thread(&blTimer::run<blFunctor,blFunctorArguments...>,std::ref(*this),std::ref(functor));

        return true;
    }
    else
    {
        // This means the thread had already
        // started and not finished yet

        return false;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blFunctor,
         typename...blFunctorArguments>

inline bool blTimer::start(blFunctor functor,blFunctorArguments... arguments)
{
    // We want this timer to only spin off
    // one thread, so we first check if the
    // timer had already started

    if(m_stop == true && m_thread.joinable() == false)
    {
        // This means the timer had been
        // stopped or not started yet, and
        // that the previous timer thread
        // has been detached or joined

        m_stop = false;
        m_pause = false;

        m_thread = std::thread(&blTimer::run<blFunctor,blFunctorArguments...>,std::ref(*this),std::ref(functor),std::ref(arguments...));

        return true;
    }
    else
    {
        // This means the thread had already
        // started and not finished yet

        return false;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blFunctor,
         typename...blFunctorArguments>

inline void blTimer::run(blFunctor functor,blFunctorArguments... arguments)
{
    // The first thing we do is get
    // the current time so that the
    // timer can fire at specified
    // intervals and we can keep track
    // of when the timer was started

    m_timeStarted = std::chrono::high_resolution_clock::now();

    auto lastTimeOfExecution = m_timeStarted;
    auto currentTime = m_timeStarted;



    // We then enter a loop that will keep
    // going until the user calls the stop
    // function

    while(m_stop == false)
    {
        // Get the current time

        currentTime = std::chrono::high_resolution_clock::now();



        // If enough time has passed (more than
        // the specified timer period), and the
        // thread has not been paused, then we
        // execute the thread callback function

        if(m_pause == false && (currentTime - lastTimeOfExecution) > m_period)
        {
            functor(arguments...);



            // Let's not forget to save the current
            // time as the last time the timer executed

            lastTimeOfExecution = currentTime;



            // We also check if the total timer
            // duration has passed only if the
            // specified timer duration is greater
            // than zero, otherwise we keep going
            // until the user stops the timer

            if( ((currentTime - m_timeStarted) > m_duration) && m_duration > std::chrono::duration<double>::zero() )
            {
                // In this case the timer has passed
                // the specified duration and the specified
                // duration was valid, so we stop the timer

                m_stop = true;
            }
        }
    }
}
//-------------------------------------------------------------------



#endif // BL_TIMER_HPP
