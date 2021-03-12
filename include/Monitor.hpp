
/**
    Declaration of Monitor
*/

#pragma once

#include <atomic>               // std::atomic
#include <boost/signals2.hpp>   // std::boost::signals2::connection
#include <condition_variable>   // std::condition_variable
#include <mutex>                // std::mutex
#include <thread>               // std::thread

/**
    Class to monitor the eyes. This class will track the
    eyes and emit a signal when the user blinks.
*/
class Monitor
{
public:

    Monitor();

    ~Monitor() = default;

    void Start();

    void Stop();

    boost::signals2::connection RegisterUserBlinked
        (
        boost::signals2::signal<void ()>::slot_type const& aSlot
        );

private:

    void TrackEyes();

    void TestTrackEyes();

    // Emitted when user needs to reminded to perform this habit
    boost::signals2::signal<void ()> mUserBlinked;

    // Thread
    std::thread mThread;
    // Flag set to true when application needs to exit
    std::atomic<bool> mExitMonitoring;
    // Waits mInterval before reminding to perform habit or until woken up to exit
    std::condition_variable mCondVar;
    // Mutex for mRestCondVar
    std::mutex mCondVarMutex;
};