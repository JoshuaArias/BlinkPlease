/**
    Declaration of App
*/

#pragma once

#include <atomic>             // std::atomic
#include <boost/signals2.hpp> // std::boost::signals2::connection

class Monitor;
class Blink;
class Rest;

/**
    Runs the application. All of the objects will be initalized in this
    object and ensure signals, when emitted, are properly handled and
    forwarded to the correct objects.
*/
class App
{
public:

    App( int aBlinkInterval, int aRestInterval, int aRestDuration );

    ~App();

    void Run();

private:

    void OnUserBlinked();

    void OnBlinkReminder();

    void OnBlinkCancel();

    void OnRestReminder( int aRestDuration );

    void OnRestCancel();

    void NightLight( bool aTurnOn, int aTemperature );

    void SendNotification( int aTimeout );

    void RegisterCallbacks();

    // Flag set to true when rest habit is being enforced
    std::atomic<bool> mResting;

    // Eye tracking objects
    std::unique_ptr<Monitor> mMonitor;
    boost::signals2::connection mUserBlinkedConnection;

    // Blink habit objects
    std::unique_ptr<Blink> mBlinkHabit;
    boost::signals2::connection mBlinkReminderConnection;
    boost::signals2::connection mBlinkCancelConnection;

    // Rest habit objects
    std::unique_ptr<Rest> mRestHabit;
    boost::signals2::connection mRestReminderConnection;
    boost::signals2::connection mRestCancelConnection;
};