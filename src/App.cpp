/**
    Definition of App
*/

#include "App.hpp"

#include <iostream> // std::cin, std::getline
#include <string>   // std::string, std::to_string

#include "Blink.hpp"
#include "Monitor.hpp"
#include "Rest.hpp"

// Constants for changing night light settings
int TEMPERATURE_REST = 2500;
int TEMPERATURE_BLINK = 4000;

/**
    Constructor
*/
App::App( int aBlinkInterval, int aRestInterval, int aRestDuration )
    : mResting( false )
    , mMonitor( new Monitor() )
    , mBlinkHabit( new Blink( aBlinkInterval ) )
    , mRestHabit( new Rest( aRestInterval, aRestDuration ) )
{
    RegisterCallbacks();
}

/**
    Destructor
*/
App::~App()
{
    mMonitor.reset();
    mBlinkHabit.reset();
    mRestHabit.reset();

    mUserBlinkedConnection.disconnect();
    mBlinkReminderConnection.disconnect();
    mBlinkCancelConnection.disconnect();
    mRestReminderConnection.disconnect();
    mRestCancelConnection.disconnect();

    // Reset temperature to default
    system( "gsettings set org.gnome.settings-daemon.plugins.color night-light-temperature 4000" );

    NightLight( false, -1 );
}

/**
    Begins the application by starting the eye tracking and habits,
    waits for user to exit, and finally cleans up when user exists
*/
void App::Run()
{
    // Start Applications
    mMonitor->Start();
    mBlinkHabit->Start();
    mRestHabit->Start();

    // Waits until user hits enter
    std::string in;
    std::getline( std::cin, in );

    // Stops Applications
    mMonitor->Stop();
    mBlinkHabit->Stop();
    mRestHabit->Stop();
}

/**
    Slot for Monitor::mUserBlinked signal

    Forwards signal from Monitor that user blinked to mBlinkHabit
*/
void App::OnUserBlinked()
{
    mBlinkHabit->OnUserBlinked();
}

/**
    Slot for Blink::mRemind signal

    Turns ON monitor's night filter (temperature = 5000)
*/
void App::OnBlinkReminder()
{
    if( !mResting )
    {
        NightLight( true, TEMPERATURE_BLINK );
    }
}

/**
    Slot for Blink::mCancel signal

    Turns OFF monitor's night filter
*/
void App::OnBlinkCancel()
{
    if( !mResting )
    {
        NightLight( false, -1 );
    }
}

/**
    Slot for Rest::mRemind signal

    Turns ON monitor's night lights and sends a notification
*/
void App::OnRestReminder( int aRestDuration )
{
    mResting = true;
    SendNotification( aRestDuration );
    NightLight( true, TEMPERATURE_REST );
}

/**
    Slot for Rest::mCancel signal

    Turns OFF monitors't night light
*/
void App::OnRestCancel()
{
    mResting = false;
    NightLight( false, -1 );
}

/**
    Sets the state of the desktop night light
*/
void App::NightLight( bool aTurnOn, int aTemperature )
{
    if( aTurnOn )
    {
        // Set temperature for night light
        std::string setTempCommand = "gsettings set org.gnome.settings-daemon.plugins.color night-light-temperature ";
        setTempCommand += std::to_string( aTemperature );
        system( setTempCommand.c_str() );
    }

    // Switches light on or off
    std::string onOffCommand = "gsettings set org.gnome.settings-daemon.plugins.color night-light-enabled ";
    onOffCommand += ( aTurnOn ) ? "true" : "false";
    system( onOffCommand.c_str() );
}

/**
    Sends notification to user to inform them to rest their eyes
*/
void App::SendNotification( int aTimeout )
{
    // Note: notify-send ignores timeout, it is a known bug
    std::string command = "notify-send -u critical \"Rest your eyes!\" -t " + std::to_string( aTimeout*1000 );
    system( command.c_str() );
}

/**
    Registers all callbacks for signals
*/
void App::RegisterCallbacks()
{
    // Register to get callbacks for Monitor's Blinked signal
    boost::signals2::signal<void ()>::slot_type userBlinkedSlot( &App::OnUserBlinked, this );
    mUserBlinkedConnection = mMonitor->RegisterUserBlinked( userBlinkedSlot );

    // Register to get callbacks for Blink's Remind signal
    boost::signals2::signal<void ()>::slot_type blinkReminderSlot( &App::OnBlinkReminder, this );
    mBlinkReminderConnection = mBlinkHabit->RegisterBlinkReminder( blinkReminderSlot );

    // Register to get callbacks for Blink's Cancel signal
    boost::signals2::signal<void ()>::slot_type blinkCancelSlot( &App::OnBlinkCancel, this );
    mBlinkCancelConnection = mBlinkHabit->RegisterBlinkCancel( blinkCancelSlot );

    // Register to get callbacks for Rest's Remind signal
    boost::signals2::signal<void ( int aRestDuration )>::slot_type restReminderSlot( &App::OnRestReminder, this, _1 );
    mRestReminderConnection = mRestHabit->RegisterRestReminder( restReminderSlot );

    // Register to get callbacks for Rest's Remind signal
    boost::signals2::signal<void ()>::slot_type restCancelSlot( &App::OnRestCancel, this );
    mRestCancelConnection = mRestHabit->RegisterRestCancel( restCancelSlot );
}