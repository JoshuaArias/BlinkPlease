/**
    Runs app

    Helps the user maintain healthy eye habits. These habits include blinking frequently and
    resting their eyes by closing them for 20 seconds every 20 minutes.

    When the application begins the webcam will be turned on and detect when the user blinks. If
    the time from the last blink exceeds 5 seconds, a notification will be sent to remind the user
    to blink. The screen's night light will be turned on at a light intensity until the user blinks
    again. Every 20 minutes an strong notification will be sent to the user to remind them to rest
    their eyes for 20 seconds. The screen's night light will be turned on at a strong intensity
    until the required 20 seconds of rest has been completed. The user will be able to set the
    cadence of the reminders to blink and rest and the duration of the resting period.
*/

#include <cstdlib>  // atoi

#include "App.hpp"

int main( int argc, char* argv[] )
{
    int blinkInterval;
    int restInterval;
    int restDuration;

    if( argc != 4 )
    {
        blinkInterval = 4;       // 4 seconds
        restInterval  = 20 * 60; // 20 minutes
        restDuration  = 20;      // 20 seconds
    }
    else
    {
        blinkInterval = atoi( argv[1] );
        restInterval  = atoi( argv[2] );
        restDuration  = atoi( argv[3] );
    }

    App application( blinkInterval, restInterval, restDuration );
    application.Run();

    return 0;
}