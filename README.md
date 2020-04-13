# timeOS
Arduino based firmware for the Pinetime watch from Pine64

## intro
There are already various operating systems in development stage for the Pinetime, but I couldn't find something that was straightforward to understand, and to develop for. Since the PineTime's CPU is already supported in Arduino, I've put toghether a bunch of code to hopefully show the capabilities of the watch.

### New to the hardware?
Take a look at: https://wiki.pine64.org/index.php/PineTime to get to know it better.
You can also find different OS implementations linked here: https://wiki.pine64.org/index.php/PineTime#Development_efforts

### Using this repo
Clone the repository, and install the following libraries:
* Adafruit ST7789 library
* Adafruit GFX Library (and dependencies)
* HRS3300 Library (https://github.com/atc1441/HRS3300-Arduino-Library)
  * This library includes a closed source blob. Make sure you follow installation instructions correctly
* Install Sandeep Mistry's NRF5 Core: https://github.com/sandeepmistry/arduino-nRF5

### Hardware support status:
* Boot: Yes
* Display: Yes
* Battery: Partial (gets voltage, shows percentage)
* Touch screen: Yes
* Side key: Yes
* Vibrator: Yes
* BMA421 Accelerometer: No
* HRS3300 Heart rate monitor: Yes
* Real Time Clock: Yes
* Bluetooth: Yes (just started but no work done on it)
* External SPI Flash: No
* Power management: Not even tried yet (too soon for that)

### Software support:
* Simple clock: Partial (shows time, can't set it)
* Alarm: No
* Timer: No
* Stopwatch: Start/Stop (no lap support)
* Heart rate monitor: Example view (starts, can measure BPMs, doesn't save it, only shows it on screen)
* Activity tracking: No
* Settings: Dummy prototype
* Bluetooth: No (device started and visible, no work done on it yet)
* Translations: No

### Code structure:
* TimeOS.ino: Entry point
  * src/: Source tree for views and drivers
    * driver/: Classes for handling all drivers, they are loaded from Hal
    * lang/: Files with translated strings
    * views/: Application views (Clock, Alarm, Settings...)
    * hal.[cpp,h]: All the drivers are accessed from here
    * system.[cpp,h]: Used for coordination between different views and watch states

Still on an early development state, almost nothing works as it should!
