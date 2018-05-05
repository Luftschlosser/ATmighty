/*
 * This headerfile contains some #defines and const-data which defines how the MessageLog system is used within ATmighty
 */

#ifndef CONFIG_MESSAGELOGCONFIG_H_
#define CONFIG_MESSAGELOGCONFIG_H_


///Boolean switch to enable/disable logging within ATmighty
#define ATMIGHTY_MESSAGELOG_ENABLE true

///Defines the LogLevel used within ATmighty. Possible values are [NoLog/Fatal/Error/Warning/Info/Debug]
#define ATMIGHTY_MESSAGELOG_LEVEL Debug

///Defines the size of the buffer-queue used by the ATmighty MessageLog-system in Bytes [1-255]
#define ATMIGHTY_MESSAGELOG_BUFFERSIZE 128


#endif /* CONFIG_MESSAGELOGCONFIG_H_ */
