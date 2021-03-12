/**
    Definition of Blink
*/

#include "Blink.hpp"

/**
    Constructor
*/
Blink::Blink( int aInterval )
    : mInterval( aInterval )
    , mExitHabit( false )
    , mUserBlinked( false )
{
}

/**
    Begins tracking of the blink habit
*/
void Blink::Start()
{
    mThread = std::thread( &Blink::HostHabit, this );
}

/**
    Wakes up mCondVar and exits thread
*/
void Blink::Stop()
{
    mExitHabit = true;
    mCondVar.notify_one();
    if( mThread.joinable() )
    {
        mThread.join();
    }
}

/**
    Wakes up mCondVar and informs that user has blinked
*/
void Blink::OnUserBlinked()
{
    mUserBlinked = true;
    mCondVar.notify_one();
}

/**
    Registers callback for mRemind signal

    @return connection to mRemind signal
*/
boost::signals2::connection Blink::RegisterBlinkReminder
    (
    boost::signals2::signal<void ()>::slot_type const& aSlot
    )
{
    return mRemind.connect( aSlot );
}

/**
    Registers callback for mCancel signal

    @return connection to mCancel signal
*/
boost::signals2::connection Blink::RegisterBlinkCancel
    (
    boost::signals2::signal<void ()>::slot_type const& aSlot
    )
{
    return mCancel.connect( aSlot );
}

/**
    Manages habit of blinking at least once every mInterval seconds
*/
void Blink::HostHabit()
{
    while( !mExitHabit )
    {
        std::unique_lock<std::mutex> lock( mCondVarMutex );
        if( mCondVar.wait_for
            (
            lock,
            std::chrono::seconds( mInterval ),
            [this]() { return mExitHabit || mUserBlinked; }
            ) )
        {
            // if woken up
            mCancel();
            mUserBlinked = false;
        }
        else
        {
            // if timed out
            mRemind();
        }
    }
}