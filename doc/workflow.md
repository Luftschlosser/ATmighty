# ATmighty Workflow

The ATmighty library is developed using Eclipse Oxygen (4.7.3a) under Ubuntu 16.04 (LTS), using the avr-eclipse plugin. In the current version an ArduinoMega2560 with an ATmega2560 microcontroller is used as a development board.

## Development Environment

### Setting up Eclipse for AVR-development

#### Prerequisites

The following programs/tools must be installed in order to set up the development environment under Ubuntu 16.04:
* eclipse neon/oxygen (C++)
* gcc-avr: `sudo apt-get install gcc-avr`
* avr-libc: `sudo apt-get install avr-libc`
* avrdude: `sudo apt-get install avrdude`
* binutils-avr: `sudo apt-get install binutils-avr`

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
  * Port: "/dev/ttyUSBn" or "/dev/ttyACMn" (n = 0,1,2,...,n) - (may be different on other linux-machines)
  * Baudrate: "115200" (If not working, see what the Arduino-IDE uses for your board)
  * Other options: "-v -D" (If not working, see what the Arduino-IDE uses for your board)
* Under Project/Properties/AVR/AVRDude/Advanced:
  * Check "Disable device signature check" (only necessary for some Arduino-boards)
* Under Project/Properties/AVR/TargetHardware:
  * Choose your MCU-Type (ATmega2560 in my case)
  * Choose your MCU-clock-frequency (in Hz, 16000000 in my case)
* Under Window/Preferences/AVR/AVRDude
  * check "Use custom config file" and set path to the Arduino-IDE's avrdude.conf file. (found at `/home/user/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf` on my machine)
* Under Project/Properties/C-C++Build/Settings: (make sure to adjust all build-configurations!)
  * Under "Additional Tools in Toolchain"
    * Check "Generate HEX file for Flash memory" and "Generate HEX file for EEPROM"
    * Uncheck "AVRDude"

If you want to develop software that is compatible with Arduino as well, you additionally have to configure the following properties:
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

#### Enabling C++11 Support

Some of the ATmighty features require the use of the C++11 standard. The Gcc compiler must be told explicitly to use this version.
* Under Project/Properties/C-C++Build/Settings: (make sure to adjust all build-configurations!)
  * Under "AVR C++ Compiler" -> "Miscellaneous":
    * Add `-std=c++11` to the "Other flags" field.
    
### Using Static Libraries

If you want to use a static AVR-library (like ATmighty) in your project, you have to configure the following properties for the project which uses the library:

* Under Project/Properties/C-C++Build/Settings: (make sure to adjust all build-configurations!)
  * Under "AVR Compiler" -> "Directories":
    * Add the path to the library project folder to the "Include Paths (-I)". Example: `"${workspace_loc:/ATmighty}"`
  * Under "AVR C++ Compiler" -> "Directories":
    * Add the path to the library project folder to the "Include Paths (-I)". Example: `"${workspace_loc:/ATmighty}"`
  * Under "AVR C++ Linker" -> "Libraries":
    * Add the name of the library to the "Libraries (-l)". Example: `ATmighty`
    * Add the path to the folder of the library-file (*.a) to the "Libraries Path (-L)": `"${workspace_loc:/ATmighty/Debug}"`

## Debugging Environment

### On-Chip Debugging

#### Prerequisites

The following programs/tools must be installed in order to use on-device debugging under Ubuntu 16.04:
* gdb-avr `sudo apt-get install gdb-avr`
* avarice `sudo apt-get install avarice`
* eclipse gdb-hardware-debugging-plugin (only necessary when debugging directly within eclipse)
  * Install by clicking "Help -> Install new Software" in eclipse.
  * Use URL http://download.eclipse.org/releases/oxygen (or http://download.eclipse.org/releases/neon) as plugin-source.
  * Select "C/C++ GDB Hardware Debugging" from "Mobile and Device Development".
  * Follow the installation routine.

The following Hardware is necessary in order to use on-device debugging with JTAG under Ubuntu 16.04:
* Atmel "JTAG-ICE mkII"
* JTAG/ISP Adapter
* ATmega2560 with enabled JTAG-interface (see fuse-settings below)

If using an ArduinoMega2560 as development-board it is necessary to enable the JTAG-interface by setting the correct fuses first.
The JTAG-interface will use the analog pins A4, A5, A6 and A7 of the ArduinoMega2560, so keep on mind that these pins will be unavailable for other use after anabling JTAG debugging. The fuses can be set using the "Atmel JTAG-ICE mkII", hooked up to the ISP-interface of the board.
The fuses must be set to: E-Fuse: `0xFD`, H-Fuse: `0x00`, L-Fuse: `0xFF`. This can be achieved using the following commands:
* `avrdude -c jtag2isp -p m2560 -v -U efuse:w:0b11111101:m`
* `avrdude -c jtag2isp -p m2560 -v -U hfuse:w:0b00000000:m`
* `avrdude -c jtag2isp -p m2560 -v -U hfuse:w:0b11111111:m`

#### Generating debuggable code

In order to compile code which can be disassembled by avr-gdb, compile-option "-g" has to be used. When using eclipse make sure to set the following properties:

* Under Project/Properties/AVR/AVRDude:
  * Add a new programmer configuration:
    * Choose programmer hardware "Atmel JTAG ICE mkII (jtag2)"
* Under Project/Properties/Additional Tools in Toolchain: (make sure to adjust the "Debug"-build-configuration!)
  * Check "Print Size"
* Under Project/Properties/C-C++Build/Settings:
  * Under "AVR Compiler" -> "Debugging"
    * Choose "No debugging info" (As gcc-parameters "-g1", "-g2", "-g3" don't work for some reason)
  * Under "AVR Compiler" -> "Miscellaneous"
    * Add "-g" to the "other flags"
  * Under "AVR C++ Compiler" -> "Debugging"
    * Choose "No debugging info" (As gcc-parameters "-g1", "-g2", "-g3" don't work for some reason)
  * Under "AVR C++ Compiler" -> "Miscellaneous"
    * Add "-g" to the "other flags"
* Under Project/Properties/AVR-Create-Extended-Listing:
  * remove the leading "-" from "avr-objdump" if there is one.
* Under Project/Properties/AVR Create Flash Image:
  * remove the leading "-" from "avr-objcopy" if there is one.
* Under Project/Properties/Print Size:
  * remove the leading "-" from "avr-size" if there is one.
  
In order for the debugger to trace the code correctly, it might be necessary to disable all optimizations under "Project/Properties/C-C++Build/Settings/AVR(C++)Compiler/Optimizations".
  
Now it should be possible to program the ATmega2560 via JTAG using the Atmel JTAG ICE mkII. Make sure the wiring is correct and that both, the JTAG-ICE-debugger and the ArduinoMega2560 are plugged in via USB, as the debugger does not provide power to the microcontroller.

#### Debugging using the terminal

Use the following commands to debug the ATmega2560 via the terminal:

* Start avarice in seperate terminal: `avarice  -2 --jtag usb  :4242`
* Start avr-gdb with the currently running .elf-file: `avr-gdb /path/program.elf`
* Connect gdb with avarice: `(gdb) target remote localhost:4242`

Now it should be possible to debug the program using common gdb-commands.

#### Debugging using eclipse

In order to debug directly from eclipse it is necessary to set up a correct debug-configuration. (Make sure to have the gdb-hardware-debugging-plugin installed).

* Click the Arrow next to the debug-button, choose "Debug configurations"
  * Add a new "GDB Hardware Debugging"-configuration
    * Choose your program's .elf-file as C/C++-Application
    * set "avr-gdb" as GDB-Command under "Debugging" and set the correct portnumber (usually localhost:4242).
    * Under "Startup":
      * Uncheck "Reset and Delay"
      * Uncheck "Halt"
      * Uncheck "Load image"
      * Check "Load symbols"
      * Set your program's .elf-file as "Use file"
      * Set breakpoint at "main"
      * Check "Resume"

To debug from eclipse, start an avarice instance manually via a terminal, then select your debug-configuration from the dropdown-menu next to the debug-button in eclipse.

#### Unsolved Debugging issues

For some reason the On-Chip-Debugging doesn't work correctly and I was not able to find any solution yet. So here is at least a description of the problems:

* GDB seems to assign wrong C/C++ linenumbers to the programcounter-value. (skipping lines, random branching/looping). As a consequence, breakpoints don't work as they should and it becomes very hard to trace the code execution. When viewing the disassembled code, everythings seems to work fine.
This can happen because of compiler-optimizations, but it happened also when all optimizations where turned off.
* When running uninterrupted, the program resets after a short ammount of time. This does not happen when singlestepping. After each reset, the MCUSR-register holds the value 29 (0x1D) - from my understanding, this should be an invalid value. An unhandled interrupt is also not the problem, as I set the BADISR_vect to an empty ISR-routine. This problem occurs always when the ATmega2560 was programmed via JTAG, not only when debugging. The same program runs fine when using the standard Arduino-programmer.

Please write a comment if you find a solution to any of these problems.

### Serial Terminal

In order to communicate with the connected Arduino-board via USART (over USB) its nice to have a serial-terminal ready to use inside eclipse.

#### Installing the necessary plugins

Go to “Help” -> “Install new Software” and install the "TM Terminal" plugin from the following URL: http://download.eclipse.org/releases/oxygen (or http://download.eclipse.org/releases/neon). The plugin can be found in the category "General Purpose Tools".

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
#### Usage

The terminal can be opened in Eclipse under Window/ShowView/Other/Terminal/Terminal. When opening a new Terminal, use Mode "Serial" and set the Port to "/dev/ttyUSBn" or "/dev/ttyACMn" (n = 0,1,2,...,n) - (may be different on other linux-machines).

Beware! In order to program the board via it's Serial connection you must disconnect the serial terminal first or you will get a avrdude timeout error.

## Sources (additional instruction sites)

* Setting up Eclipse for AVR-Development: https://www.mikrocontroller.net/articles/AVR_Eclipse
* Setting up Eclipse with avr-eclipse plugin: http://www.langeder.org/setup-eclipse-ide-for-avr-programming-on-ubuntu/
* Using Eclipse for Arduino-Development: https://playground.arduino.cc/Code/Eclipse#YourFirstArduinoProject
* Bug "Empty MCU-Type list": https://github.com/mnlipp/avr-eclipse-fork/issues/1
* Using the serial terminal in eclipse: https://mcuoneclipse.com/2017/10/07/using-serial-terminal-and-com-support-in-eclipse-oxygen-and-neon/
* Bugfix for rxtx-plugin: http://karibe.co.ke/2014/03/eclipse-terminal-plugin-serial-port-console-in-linux/
* Using Static Libraries for AVR: https://electronics.stackexchange.com/questions/27325/how-do-i-create-a-static-library-for-the-avr-atmega328p
* Debug-wiring and JTAG-Fuses: http://ardupilot.org/dev/docs/jtag.html
* AVR-debugging in general: http://avr-eclipse.sourceforge.net/wiki/index.php/Debugging#AVaRICE_2
* GDB-AVR debugging: http://luniks.net/avr-debug.jsp
