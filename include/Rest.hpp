
/**
    Declaration of Rest
*/

#pragma once

#include <boost/signals2.hpp>   // std::boost::signals2::connection
#include <condition_variable>   // std::condition_variable
#include <mutex>                // std::mutex
#include <thread>               // std::thread

/**
    Rest manages the habit of resting your eyes periodically. Every mInterval
    seconds, a reminder to reset eyes for aRestDuration second.
*/
class Rest
{
public:

    Rest( int aInterval, int aRestDuration );

    ~Rest() = default;

    void Start();

    void Stop();

    boost::signals2::connection RegisterRestReminder
        (
        boost::signals2::signal<void ( int aRestDuration )>::slot_type const& aSlot
        );

    boost::signals2::connection RegisterRestCancel
        (
        boost::signals2::signal<void ()>::slot_type const& aSlot
        );

private:

    void HostHabit();

    // This habit must be performed once every mInterval
    int mInterval;
    // User must rest for mRestDuration seconds
    int mRestDuration;

    // Emitted after mInterval seconds to remind user to rest for mRestDuration seconds
    boost::signals2::signal<void ( int aRestDuration )> mRemind;
    // Emitted after user has rested for mRestDuration seconds
    boost::signals2::signal<void ()> mCancel;

    // Thread to host rest habit
    std::thread mThread;
    // Flag set to true when application needs to exit
    std::atomic<bool> mExitHabit;
    // Waits mInterval or until woken up for reminder or exit
    std::condition_variable mCondVar;
    // Mutex for mRestCondVar
    std::mutex mCondVarMutex;
};