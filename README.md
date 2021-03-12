# BlinkPlease

BlinkPlease is a command line application that reminds users to maintain good eye
habits that can help prevent dry eye symptoms from extended time on the computer.
The eye habits recommended are to blink frequently and to rest your eyes by closing
them periodically. The default cadence for these habits are blinking at least once
every 4 seconds and closing your eyes for 20 seconds every 20 minutes. A webcam will
be used to detect when the user blinks. If the user does not blink at the given
interval, the screen's night light will be turned on until they blink. When the user
is to rest, the screen's night light will be turned on at a high intensity.

## Installation

Install opencv
```bash
sudo apt-get install libopencv-dev
```
Build project with cmake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

```bash
cd build
./program
./program [blinkInterval, restInterval, restDuration]
```
