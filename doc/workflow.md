# ATmighty Workflow

## Development Environment

The ATmighty library is developed using Eclipse Neon.3 (4.6.3) under Ubuntu 16.04 (LTS), using the avr-eclipse plugin. In the current version an ArduinoMega2560 with an ATmega2560 microcontroller is used as a development board.

### Setting up Eclipse for AVR-development

#### Prerequisites

The following programs/tools must be installed in order to set up the development environment under Ubuntu 16.04:
* eclipse neon (C++)
* gcc-avr: `sudo apt-get install gcc-avr`
* avr-libc: `sudo apt-get install avr-libc`
* avrdude: `sudo apt-get install avrdude`
* binutils-avr: `sudo apt-get install binutils-avr`
* gdb-avr (optional, for JTAG-debuging): `sudo apt-get install gdb-avr`
* avarice (optional, for JTAG-debuging): `sudo apt-get install avarice`

#### Installing the avr-eclipse-plugin

The avr-eclipse plugin can be installed by opening eclipse, going to “Help” -> “Install new Software”, entering the URL http://avr-eclipse.sourceforge.net/updatesite/ as new plugin source, selecting the plugin and clicking "Next" until the installation is finished.

When creating a new "AVR Cross Target Application" or a "AVR Cross Target static library" it might occur that there are no MCU-Types selectable from the dropdown menu. This bug happens because the avr-eclipse plugin can't deal with localized versions of gcc-avr. To fix this problem, rename the avr-gcc file (usually under `/usr/bin/`) into "avr-gcc-real" and creating an executable bash-file named "avr-gcc" with the following content at the same location.
```
#!/bin/bash

LC_MESSAGES=C avr-gcc-real $@
```
More informations can be found here: https://github.com/mnlipp/avr-eclipse-fork/issues/1

Another bug may occur that makes eclipse keep using the wrong MCU-Type for compiling and programming. This happened to me when I had java- and c/c++-projects in the same workspace. Switching the workspace to a c/c++-exclusive one solved the problem.

#### Configuring the project for an Arduino-Board

The following values are to be used to program an ArduinoMega2560-board directly from eclipse using it's native programmer. The values can be adjusted for other boards/programmers as well - it should be self-explanatory.
* Under Project/Properties/AVR/AVRDude/Programmer/Programmer-Configuration:
  * Programmer Hardware: "Wiring" (If not working, see what the Arduino-IDE uses for your board)
  * Port: "/dev/ttyUSBn" (n = 0,1,2,...,n) - (may be different on other linux-machines)
  * Baudrate: "115200" (If not working, see what the Arduino-IDE uses for your board)
  * Other options: "-v -D" (If not working, see what the Arduino-IDE uses for your board)
* Under Project/Properties/AVR/AVRDude/Advanced:
  * Check "Disable device signature check" (only necessary for some Arduino-boards)
* Under Project/Properties/AVR/TargetHardware:
  * Choose your MCU-Type (ATmega2560 in my case)
  * Choose your MCU-clock-frequency (in Hz, 16000000 in my case)
* Under Window/Preferences/AVR/AVRDude
  * check "Use custom config file" and set path to the Arduino-IDE's avrdude.conf file. (found at `/home/user/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf` on my machine)

If you want to develop software that is compatible with the Arduino-IDE as well, you additionally have to configure the following properties:
* Under Project/Properties/C-C++Build/Settings: (make sure to adjust all build-configurations!)
  * Under "AVR Compiler" -> "Optimizations":
    * Set the Optimization Level to "Size Optimizations"
    * Turn off "Pack structs (-fpack-struct)" and "Short enums (fshort-enum)"
    * Turn on "Each function in it's own section (-ffunction-sections)" and "Each data-item in it's own section (-fdata-sections)"
  * Under "AVR Compiler" -> "Language standard":
    * Turn off "char is unsigned" and "bitfields are unsigned"
  * Under "AVR C++ Compiler" -> "Optimizations":
    * Set the Optimization Level to "Size Optimizations"
    * Turn off "Pack structs (-fpack-struct)" and "Short enums (fshort-enum)"
    * Turn on "Each function in it's own section (-ffunction-sections)" and "Each data-item in it's own section (-fdata-sections)"
  * Under "AVR C++ Compiler" -> "Language standard":
    * Turn off "char is unsigned" and "bitfields are unsigned"
    * Enable "don't use exceptions"

### Setting up Eclipse to have a serial terminal

In order to communicate with the connected Arduino-board via USART (over USB) its nice to have a serial-terminal ready to use inside eclipse.

#### Installing the necessary plugins

Go to “Help” -> “Install new Software” and install the "TM Terminal" plugin from the following URL: http://download.eclipse.org/releases/neon. The plugin can be found in the category "General Purpose Tools".

Also install the plugin "RXTX End-User Runtime" from the following URL: http://rxtx.qbang.org/eclipse/. The plugin can be found in the category "RXTX 2.1-7r4".

As soon as you receive your first character via Serial, eclipse will crash due to a bug in the RXTX-plugin. To solve this problem, follow these steps:
* Install "librxtx-java" on your machine: 
```
sudo apt-get install librxtx-java
```
* Remove the "librxtxSerial.so" file from eclipse plugin folder: 
```
rm ~/.p2/pool/plugins/gnu.io.rxtx.linux.x86_64_2.1.7.3_v20071015/os/linux/x86_64/librxtxSerial.so
```
* Set a SoftLink named "librxtxSerial.so" pointing to "/usr/lib/jni/librxtxSerial.so" in that folder: 
```
ln -s /usr/lib/jni/librxtxSerial.so ~/.p2/pool/plugins/gnu.io.rxtx.linux.x86_64_2.1.7.3_v20071015/os/linux/x86_64/librxtxSerial.so
```

The terminal can be opened in Eclipse under Window/ShowView/Other/Terminal/Terminal. When opening a new Terminal, use Mode "Serial" and set the Port to "/dev/ttyUSBn" (n = 0,1,2,...,n) - (may be different on other linux-machines).

### Sources (additional instruction sites)

* Setting up Eclipse for AVR-Development: https://www.mikrocontroller.net/articles/AVR_Eclipse
* Setting up Eclipse with avr-eclipse plugin: http://www.langeder.org/setup-eclipse-ide-for-avr-programming-on-ubuntu/
* Using Eclipse for Arduino-Development: https://playground.arduino.cc/Code/Eclipse#YourFirstArduinoProject
* Bug "Empty MCU-Type list": https://github.com/mnlipp/avr-eclipse-fork/issues/1
* Using the serial terminal in eclipse: https://mcuoneclipse.com/2017/10/07/using-serial-terminal-and-com-support-in-eclipse-oxygen-and-neon/
* Bugfix for rxtx-plugin: http://karibe.co.ke/2014/03/eclipse-terminal-plugin-serial-port-console-in-linux/
