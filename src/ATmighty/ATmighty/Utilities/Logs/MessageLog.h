/*!
 * This headerfile is µC-independent and can be used directly with all AVR's.
 * This headerfile also contains the whole implementation of the MessageLog, as this is needed in order to implicitly instantiate a template class.
 */

#ifndef ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_
#define ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_


#include <stdint.h>
#include "ATmighty/DataStructures/IoQueue.h"
#include "Config/MessageLogConfig.h"


namespace MessageLogWriter
{
	class Base;
}


/*
 * This enum defines the different loglevels usable with \see MessageLog
 */
enum LogLevel : uint8_t {
	NoLog = 0,
	Fatal = 1,
	Error = 2,
	Warning = 3,
	Info = 4,
	Debug = 5
};


/*!
 * This class represents a universal MessageLog system with changeable output. It Logs Messages as string.
 * The template parameter is of type enum LogLevel and specifies the LogLevel of this MessageLog system.
 */
template<LogLevel OutputLevel = LogLevel::ATMIGHTY_MESSAGELOG_LEVEL> class MessageLog
{
	private:
		//the buffer which holds the messages to write out
		IoQueue<char> bufferQueue;

		//The MessageLogWriter currently used to write out messages
		MessageLogWriter::Base *writer;


		//Buffer-functions automatically load datatypes into bufferQueue
		void buffer(char c);
		void buffer(const char* msg, bool rom);
		void buffer(uint8_t num);
		void buffer(int8_t num);
		void buffer(bool val);

		//Buffers the init-character-sequence of a  message into the bufferQueue (depends on the Level of the Message specified by the template parameter)
		template<LogLevel Level> void bufferMessageStart();

		//Buffers the end-characters of a message into the bufferQueue
		void bufferMessageEnd();


		//logging-routines for each supported type: (variadic template approach)

		//const char* type
		template<typename... T> void logVariadic(bool rom, const char* firstArg, const T&... args)
		{
				buffer(firstArg, rom);
				logVariadic(rom, args...);
		}
		//uint8_t type
		template<typename... T> void logVariadic(bool rom, uint8_t firstArg, const T&... args)
		{
				buffer(firstArg);
				logVariadic(rom, args...);
		}

		//int8_t type
		template<typename... T> void logVariadic(bool rom, int8_t firstArg, const T&... args)
		{
				buffer(firstArg);
				logVariadic(rom, args...);
		}
		//char type
		template<typename... T> void logVariadic(bool rom, char firstArg, const T&... args)
		{
				buffer(firstArg);
				logVariadic(rom, args...);
		}
		//bool type
		template<typename... T> void logVariadic(bool rom, bool firstArg, const T&... args)
		{
				buffer(firstArg);
				logVariadic(rom, args...);
		}


		//end of variadic template recursion -> ends log-message
		void logVariadic(bool rom)
		{
			bufferMessageEnd();
		}


	public:
		/*!
		 * Constructor
		 * \param bufferSize Specifies the size of the internal bufferQueue queue.
		 */
		MessageLog(uint8_t bufferSize);

		/*!
		 * Destructor
		 * Disconnects the currently used MessageLogWriter
		 */
		~MessageLog();

		/*!
		 * Sets a new MessageLogWriter which shall write out everything which is logged into this MessageLog instance
		 * MessageLog doesn't take any responsibility in freeing the MessageLogWriter-object when the writer is changed or MessageLog is deconstructed!
		 * \param writer The writer object to set (derived from MessageLogWriter::Base)
		 */
		void setWriter(MessageLogWriter::Base *writer);

		/*!
		 * Logs an arbitrary number of data-items of arbitrary types with a specified loglevel.
		 * The loglevel is specified by the (first, others are redundant) template argument.
		 * Usage example: 'messageLogObj.log<LogLevel::Error>(false, 8, " times ", 5, " is even: ", true);' -> will print: "E: 8 times 5 is even: True\r\n".
		 * \param rom The first parameter of the parameter-list must always be a boolean which specifies wether
		 * const char* -types (strings) are stored in Programspace (true) or in Ram (false).
		 * All const char* -types in the argument list must be stored in the same memory, scrambling storage types is not possible!
		 */
		template<LogLevel InputLevel, typename Tfirst, typename... Tothers> void log(bool rom, const Tfirst& firstItem, const Tothers&... args)
		{
			if (OutputLevel >= InputLevel)
			{
				bufferMessageStart<InputLevel>();
				logVariadic(rom, firstItem, args...);
			}
		}


		///Returns an instance of the default MessageLog
		inline static MessageLog<>& DefaultInstance()
		{
			static MessageLog<> defaultInstance(ATMIGHTY_MESSAGELOG_BUFFERSIZE);
			return defaultInstance;
		}
};


#endif /* ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_ */
