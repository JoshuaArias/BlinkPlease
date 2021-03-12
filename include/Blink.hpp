
/**
    Declaration of Blink
*/

#pragma once

#include <atomic>              // std::atomic
#include <boost/signals2.hpp>  // boost::signals2::connection
#include <condition_variable>  // std::condition_variable
#include <mutex>               // std::mutex
#include <thread>              // std::thread

/**
    Blink manages the habit of blinking consistently. Every mInterval seconds,
    if the user hasn't blinked, a reminder signal will be sent.
*/
class Blink
{
public:

    Blink( int aInterval );

    ~Blink() = default;

    void Start();

    void Stop();

    void OnUserBlinked();

    boost::signals2::connection RegisterBlinkReminder
        (
        boost::signals2::signal<void ()>::slot_type const& aSlot
        );

    boost::signals2::connection RegisterBlinkCancel
        (
        boost::signals2::signal<void ()>::slot_type const& aSlot
        );

private:

    void HostHabit();

    // This habit must be performed once every mInterval
    int mInterval;

    // Emitted when user needs to reminded to blink
    boost::signals2::signal<void ()> mRemind;
    // Emitted when user has blinked
    boost::signals2::signal<void ()> mCancel;

    // Thread to host blink habit
    std::thread mThread;
    // Flag set to true when application needs to exit
    std::atomic<bool> mExitHabit;
    // Flag set to true when user has blinked
    std::atomic<bool> mUserBlinked;
    // Waits mInterval before waking up to emit a signal or exit
    std::condition_variable mCondVar;
    // Mutex for mRestCondVar
    std::mutex mCondVarMutex;
};