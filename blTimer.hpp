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
// NOTE: This class is defined within the blTimerAPI namespace
//-------------------------------------------------------------------
namespace blTimerAPI
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class blTimer
{
public: // Constructors and destructor



    // Default constructor

    blTimer();



    // Destructor

    virtual ~blTimer();



    // Eliminate copy construction and
    // assignment operator

    blTimer(blTimer const&) = delete;

    blTimer&                                                    operator=(blTimer const&) = delete;



public:  // Public functions



    // Functions used to set/get the timer
    // period in seconds

    void                                                        setPeriod(const double& periodExpressedInSeconds);
    void                                                        setPeriod(const std::chrono::duration<double>& period);
    const std::chrono::duration<double>&                        getPeriod()const;




    // Functions used to set/get the timer's
    // total duration

    void                                                        setDuration(const double& timerDurationInSeconds);
    void                                                        setDuration(const std::chrono::duration<double>& duration);
    const std::chrono::duration<double>&                        getDuration()const;



    // Functions used to set/get the maximum
    // number of times that the timer fires

    void                                                        setMaximumNumberOfTimesTheTimerFires(const int& maximumNumberOfTimesTheTimerFires);
    const int&                                                  getMaximumNumberOfTimesTheTimerFires()const;



    // Functions used to get the current
    // time/firings while the thread is
    // running

    const int&                                                  getCurrentNumberOfTimesTheTimerFired()const;
    const std::chrono::high_resolution_clock::time_point&       getTimeTheThreadStartedRunning()const;



    // Functions used to stop the thread and
    // either join the calling thead or just
    // be detached

    void                                                        stopAndJoin();
    void                                                        stopAndDetach();
    void                                                        join();
    void                                                        detach();



    // pause function will signal the
    // thread to pause its execution to
    // later be restarted again

    void                                                        pause();



    // Functions used ot start the
    // parallel thread

    template<typename blFunctor,
             typename...blFunctorArguments>

    bool                                                        start(blFunctor functor);

    template<typename blFunctor,
             typename...blFunctorArguments>

    bool                                                        start(blFunctor functor,
                                                                      blFunctorArguments... arguments);



protected:  // Protected functions



    // Run function will call the registered
    // callbacks as the thread is being executed
    // this function takes as an argument a functor
    // with functor arguments and will call that
    // functor with passed arguments repeatedly
    // every time the timer hits

    template<typename blFunctor,
             typename...blFunctorArguments>

    void                                                        run(blFunctor functor,
                                                                    blFunctorArguments... arguments);



private:  // Private variables



    // The atomic variable used to signal
    // the thread to stop its execution

    std::atomic<bool>                                           m_stop;



    // The atomic variable used to "pause"
    // the thread, meaning that if the
    // pause variable is true, the thread
    // does not execute its function

    std::atomic<bool>                                           m_pause;



    // The maximum number of times the
    // timer fires

    int                                                         m_maximumNumberOfTimesTheTimerFires;



    // The number of times the timer
    // has fired

    int                                                         m_currentNumberOfTimesTheTimerHasFired;



    // The timer period in seconds

    std::chrono::duration<double>                               m_period;



    // The total duration of the timer
    // after which the timer will be stopped

    std::chrono::duration<double>                               m_duration;



    // Time point at which timer was started

    std::chrono::high_resolution_clock::time_point              m_timeTheThreadStartedRunning;



    // The variable used to spawn parallel
    // therads

    std::thread                                                 m_thread;

};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Default constructor
//-------------------------------------------------------------------
inline blTimer::blTimer()
{
    // Default all values

    m_stop = true;
    m_pause = false;
    m_maximumNumberOfTimesTheTimerFires = -1;
    m_currentNumberOfTimesTheTimerHasFired = 0;
    m_period = std::chrono::duration< double,std::ratio<1,1> >(0);
    m_duration = std::chrono::duration<double>::zero();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------
inline blTimer::~blTimer()
{
    stopAndJoin();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Set/get functions
//-------------------------------------------------------------------
inline void blTimer::setPeriod(const double& periodExpressedInSeconds)
{
    m_period = std::chrono::duration< double,std::ratio<1,1> >(periodExpressedInSeconds);
}



inline void blTimer::setPeriod(const std::chrono::duration<double>& period)
{
    m_period = period;
}



inline void blTimer::setDuration(const double& timerDurationInSeconds)
{
    m_duration = std::chrono::duration< double,std::ratio<1,1> >(timerDurationInSeconds);
}



inline void blTimer::setDuration(const std::chrono::duration<double>& duration)
{
    m_duration = duration;
}



inline void blTimer::setMaximumNumberOfTimesTheTimerFires(const int& maximumNumberOfTimesTheTimerFires)
{
    m_maximumNumberOfTimesTheTimerFires = maximumNumberOfTimesTheTimerFires;
}



inline const std::chrono::duration<double>& blTimer::getPeriod()const
{
    return m_period;
}



inline const std::chrono::duration<double>& blTimer::getDuration()const
{
    return m_duration;
}



inline const int& blTimer::getMaximumNumberOfTimesTheTimerFires()const
{
    return m_maximumNumberOfTimesTheTimerFires;
}



inline const int& blTimer::getCurrentNumberOfTimesTheTimerFired()const
{
    return m_currentNumberOfTimesTheTimerHasFired;
}



inline const std::chrono::high_resolution_clock::time_point& blTimer::getTimeTheThreadStartedRunning()const
{
    return m_timeTheThreadStartedRunning;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Functions used to stop the thread and either join the
// calling thead or just be detached
//-------------------------------------------------------------------
inline void blTimer::stopAndJoin()
{
    m_stop = true;

    if(m_thread.joinable() == true)
        m_thread.join();
}



inline void blTimer::stopAndDetach()
{
    m_stop = true;

    if(m_thread.joinable() == true)
        m_thread.detach();
}



inline void blTimer::join()
{
    if(m_thread.joinable())
        m_thread.join();
}



inline void blTimer::detach()
{
    if(m_thread.joinable())
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

    m_timeTheThreadStartedRunning = std::chrono::high_resolution_clock::now();

    auto lastTimeOfExecution = m_timeTheThreadStartedRunning;
    auto currentTime = m_timeTheThreadStartedRunning;



    // We also zero out the number of
    // times the timer has fired so far

    m_currentNumberOfTimesTheTimerHasFired = 0;



    // We then enter a loop that will keep
    // going until the user calls the stop
    // function or until the timer has fired
    // the maximum number of allowed times

    while(m_stop == false &&
          (m_currentNumberOfTimesTheTimerHasFired < m_maximumNumberOfTimesTheTimerFires || m_maximumNumberOfTimesTheTimerFires < 0))
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

            if( ((currentTime - m_timeTheThreadStartedRunning) > m_duration) && m_duration > std::chrono::duration<double>::zero() )
            {
                // In this case the timer has passed
                // the specified duration and the specified
                // duration was valid, so we stop the timer

                m_stop = true;
            }



            // Let's increase the number of times the
            // timer has fired so far

            ++m_currentNumberOfTimesTheTimerHasFired;
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// End of namespace
}
//-------------------------------------------------------------------



#endif // BL_TIMER_HPP
