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

		//Buffers the init-character-sequence of a  message into the bufferQueue (depends on the Level of the Message specified by the template parameter)
		template<LogLevel Level> void bufferMessageStart();

		//Buffers the end-characters of a message into the bufferQueue
		void bufferMessageEnd();

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
		 * Logs a normal constant string
		 * \param msg A pointer to the beginning of the 0-terminated constant string
		 * \param rom Determines wether the string is saved in Programspace (true) or in Ram (false, default)
		 */
		template<LogLevel InputLevel> void log(const char* msg, bool rom = false)
		{
			if (OutputLevel >= InputLevel)
			{
				bufferMessageStart<InputLevel>();
				buffer(msg, rom);
				bufferMessageEnd();
			}
		}

		/*!
		 * Logs a normal constant string, followed by an uint8_t integer (represented decimal)
		 * \param msg A pointer to the beginning of the 0-terminated constant string
		 * \param rom Determines wether the string is saved in Programspace (true) or in Ram (false, default)
		 * \param num The usigned integer to print after the msg
		 */
		template<LogLevel InputLevel> void log(const char* msg, uint8_t num, bool rom = false)
		{
			if (OutputLevel >= InputLevel)
			{
				bufferMessageStart<InputLevel>();
				buffer(msg, rom);
				buffer(num);
				bufferMessageEnd();
			}
		}

		/*!
		 * Logs a normal constant string, followed by an int8_t integer (represented decimal)
		 * \param msg A pointer to the beginning of the 0-terminated constant string
		 * \param rom Determines wether the string is saved in Programspace (true) or in Ram (false, default)
		 * \param num The signed integer to print after the msg
		 */
		template<LogLevel InputLevel> void log(const char* msg, int8_t num, bool rom = false)
		{
			if (OutputLevel >= InputLevel)
			{
				bufferMessageStart<InputLevel>();
				buffer(msg, rom);
				buffer(num);
				bufferMessageEnd();
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
