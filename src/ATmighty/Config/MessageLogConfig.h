/*
 * This headerfile is µC-independent and can be used directly with all AVR's.
 * This headerfile contains some #defines and const-data which defines how the MessageLog system is used within ATmighty
 */

#ifndef CONFIG_MESSAGELOGCONFIG_H_
#define CONFIG_MESSAGELOGCONFIG_H_


///Boolean switch to enable/disable logging within ATmighty
#define ATMIGHTY_MESSAGELOG_ENABLE true

///Defines the LogLevel used within ATmighty. Possible values are [NoLog/Fatal/Error/Warning/Info/Debug]
#define ATMIGHTY_MESSAGELOG_LEVEL Debug

///Defines the size of the buffer-queue used by the ATmighty MessageLog-system in Bytes [1-255]
#define ATMIGHTY_MESSAGELOG_BUFFERSIZE 127

///Defines the character-sequence which gets printed before a debug-message.
#define ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_DEBUG "D:"

///Defines the character-sequence which gets printed before a info-message.
#define ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_INFO "I:"

///Defines the character-sequence which gets printed before a warning-message.
#define ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_WARNING "W:"

///Defines the character-sequence which gets printed before a error-message.
#define ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_ERROR "E:"

///Defines the character-sequence which gets printed before a fatal-message.
#define ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_FATAL "F:"

///Defines the character-sequence which gets printed after each message.
#define ATMIGHTY_MESSAGELOG_ENDSEQUENCE "\r\n"

///Defines the character-sequence which represents the value of 'true' when logging boolean types.
#define ATMIGHTY_MESSAGELOG_SEQUENCE_TRUE "True"

///Defines the character-sequence which represents the value of 'false' when logging boolean types.
#define ATMIGHTY_MESSAGELOG_SEQUENCE_FALSE "False"


#endif /* CONFIG_MESSAGELOGCONFIG_H_ */
