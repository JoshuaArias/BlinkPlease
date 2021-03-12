/**
    Definition of Rest
*/

#include "Rest.hpp"

/**
    Constructor
*/
Rest::Rest( int aInterval, int aRestDuration )
    : mInterval( aInterval )
    , mRestDuration( aRestDuration )
    , mExitHabit( false )
{
}

/**
    Begins enforcement of rest habit
*/
void Rest::Start()
{
    mThread = std::thread( &Rest::HostHabit, this );
}

/**
    Wakes up waiting condition variable to exit thread
*/
void Rest::Stop()
{
    mExitHabit = true;
    mCondVar.notify_one();
    if( mThread.joinable() )
    {
        mThread.join();
    }
}

/**
    Registers callback for mRemind signal for rest reminders

    @return connection to mRemind signal
*/
boost::signals2::connection Rest::RegisterRestReminder
    (
    boost::signals2::signal<void ( int aRestDuration )>::slot_type const& aSlot
    )
{
    return mRemind.connect( aSlot );
}

/**
    Registers callback for mCancel signal for rest reminders

    @return connection to mCancel signal
*/
boost::signals2::connection Rest::RegisterRestCancel
    (
    boost::signals2::signal<void ()>::slot_type const& aSlot
    )
{
    return mCancel.connect( aSlot );
}

/**
    Manages habit of resting eyes every mInterval minutes for mRestDutation seconds
*/
void Rest::HostHabit()
{
    while( !mExitHabit )
    {
        std::unique_lock<std::mutex> lock( mCondVarMutex );
        if( mCondVar.wait_for
            (
            lock,
            std::chrono::seconds( mInterval ),
            [this]() { return mExitHabit || false; }
            ) )
        {
            // if woken up
            break;
        }

        mRemind( mRestDuration );

        mCondVar.wait_for
            (
            lock,
            std::chrono::seconds( mRestDuration ),
            [this]() { return mExitHabit || false; }
            );

        mCancel();
    }
}