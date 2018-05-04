/*!
 * This headerfile is µC-independent and can be used directly with all AVR's.
 * This headerfile also contains the whole implementation of the MessageLog, as this is needed in order to implicitly instantiate a template class.
 */

#ifndef ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_
#define ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_


#include <stdint.h>
#include <stdlib.h>
#include "ATmighty/DataStructures/IoQueue.h"
#include "MessageLogWriter.h"


//define default value for Loglevel and BufferSize if not defined by user
#ifndef ATMIGHTY_MESSAGELOG_LEVEL
	#define ATMIGHTY_MESSAGELOG_LEVEL NoLog //NoLog by default
#endif
#ifndef ATMIGHTY_MESSAGELOG_BUFFERSIZE
	#define ATMIGHTY_MESSAGELOG_BUFFERSIZE 64
#endif


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
template<LogLevel Level> class MessageLog
{
	private:
		//the buffer which holds the messages to write out
		IoQueue<char> bufferQueue;


		//Buffer-functions automatically load datatypes into bufferQueue
		inline void buffer(char c)
		{
			bufferQueue.push(c);
		}

		void buffer(const char* msg)
		{
			if (msg)
			{
				while(*msg != 0)
				{
					bufferQueue.push(*msg);
					msg++;
				}
			}
		}

		//Buffers the end-characters of a message into the bufferQueue
		void bufferMessageEnd()
		{
			bufferQueue.push('\r');
			bufferQueue.push('\n');
		}

	public:
		/*!
		 * Default Constructor
		 * \param bufferSize Specifies the size of the internal bufferQueue queue.
		 */
		MessageLog(uint8_t bufferSize) : bufferQueue(bufferSize) {}

		///Sets a new MessageLogWriter
		inline void setWriter(MessageLogWriter::Base& writer)
		{
			if (Level > LogLevel::NoLog)
			{
				writer.init(&bufferQueue);
			}
		}

		///Logs an Fatal Error
		void fatal(const char* msg)
		{
			if (Level >= LogLevel::Fatal)
			{
				buffer("F: ");
				buffer(msg);
				bufferMessageEnd();
			}
		}

		///Logs an Error
		void error(const char* msg)
		{
			if (Level >= LogLevel::Error)
			{
				buffer("E: ");
				buffer(msg);
				bufferMessageEnd();
			}
		}

		///Logs a Warning
		void warn(const char* msg)
		{
			if (Level >= LogLevel::Warning)
			{
				buffer("W: ");
				buffer(msg);
				bufferMessageEnd();
			}
		}

		///Logs an Info
		void info(const char* msg)
		{
			if (Level >= LogLevel::Info)
			{
				buffer("I: ");
				buffer(msg);
				bufferMessageEnd();
			}
		}

		///Logs an Debug Message
		void debug(const char* msg)
		{
			if (Level >= LogLevel::Debug)
			{
				buffer("D: ");
				buffer(msg);
				bufferMessageEnd();
			}
		}


		///Returns an instance of the default MessageLog
		inline static MessageLog& Instance()
		{
			static MessageLog<LogLevel::ATMIGHTY_MESSAGELOG_LEVEL> defaultInstance(ATMIGHTY_MESSAGELOG_BUFFERSIZE);
			return defaultInstance;
		}
};


#endif /* ATMIGHTY_UTILITIES_LOGS_MESSAGELOG_H_ */
