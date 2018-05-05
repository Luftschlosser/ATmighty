/*
 * This file implements class \see MessageLog
 */

#include "MessageLog.h"

#include <avr/pgmspace.h>
#include "MessageLogWriter.h"


//declare constant string sequences used to begin and end log-messages
const char DebugBeginSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_DEBUG;
const char InfoBeginSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_INFO;
const char WarningBeginSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_WARNING;
const char ErrorBeginSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_ERROR;
const char FatalBeginSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_BEGINSEQUENCE_FATAL;
const char EndSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_ENDSEQUENCE;
PGM_P const Phrases[] PROGMEM =
{
		EndSequence,
		FatalBeginSequence,
		ErrorBeginSequence,
		WarningBeginSequence,
		InfoBeginSequence,
		DebugBeginSequence
};
const char TrueSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_SEQUENCE_TRUE;
const char FalseSequence[] PROGMEM = ATMIGHTY_MESSAGELOG_SEQUENCE_FALSE;


//public method implementations:
template<LogLevel OutputLevel> MessageLog<OutputLevel>::MessageLog(uint8_t bufferSize) : bufferQueue(bufferSize), writer(nullptr)
{
}

template<LogLevel OutputLevel> MessageLog<OutputLevel>::~MessageLog()
{
	if (this->writer) //is there a previous MessageLogWriter in use?
	{
		this->writer->exit();
	}
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::setWriter(MessageLogWriter::Base *writer)
{
	if (OutputLevel > LogLevel::NoLog)
	{
		if (this->writer) //is there a previous MessageLogWriter in use?
		{
			this->writer->exit();
		}
		this->writer = writer;
		writer->init(&bufferQueue);
	}
}


//private method implementations:
template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(char c)
{
	bufferQueue.push(c);
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(const char* msg, bool rom)
{
	if (msg)
	{
		if (rom) //programspace
		{
			char buf = pgm_read_byte(msg);
			while (buf)
			{
				bufferQueue.push(buf);
				msg++;
				buf = pgm_read_byte(msg);
			}
		}
		else //normal
		{
			while(*msg != 0)
			{
				bufferQueue.push(*msg);
				msg++;
			}
		}
	}
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(uint8_t num)
{
	uint8_t buf = num/100;

	if (buf > 0)
	{
		bufferQueue.push('0' + buf);
	}
	num %= 100;
	buf += num/10;
	if (buf > 0)
	{
		bufferQueue.push('0' + (num/10));
	}
	num %= 10;
	bufferQueue.push('0' + num);
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(int8_t num)
{
	if (num < 0)
	{
		bufferQueue.push('-');
		num = -num;
	}
	buffer((uint8_t) num);
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(bool var)
{
	if (var)
	{
		buffer(TrueSequence, true);
	}
	else
	{
		buffer(FalseSequence, true);
	}
}

template<LogLevel OutputLevel> template<LogLevel Level> void MessageLog<OutputLevel>::bufferMessageStart()
{
	if (Level > LogLevel::NoLog)
	{
		buffer((PGM_P)pgm_read_word(&(Phrases[Level])), true);
		buffer(' ');
	}
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::bufferMessageEnd()
{
	buffer((PGM_P)pgm_read_word(&(Phrases[0])), true);
}


//Explizit instantiations of MessageLog class types
template class MessageLog<LogLevel::NoLog>;
template class MessageLog<LogLevel::Fatal>;
template class MessageLog<LogLevel::Error>;
template class MessageLog<LogLevel::Warning>;
template class MessageLog<LogLevel::Info>;
template class MessageLog<LogLevel::Debug>;

//Explizit instantiations of templated method bufferMessageStart()
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::NoLog>::bufferMessageStart<LogLevel::Debug>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::Fatal>::bufferMessageStart<LogLevel::Debug>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::Error>::bufferMessageStart<LogLevel::Debug>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::Warning>::bufferMessageStart<LogLevel::Debug>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::Info>::bufferMessageStart<LogLevel::Debug>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::NoLog>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::Fatal>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::Error>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::Warning>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::Info>();
template void MessageLog<LogLevel::Debug>::bufferMessageStart<LogLevel::Debug>();
