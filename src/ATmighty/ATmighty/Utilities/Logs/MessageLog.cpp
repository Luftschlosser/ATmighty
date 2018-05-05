/*
 * This file implements class \see MessageLog
 */

#include "MessageLog.h"

#include "MessageLogWriter.h"


//private method implementations:
template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(char c)
{
	bufferQueue.push(c);
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(const char* msg)
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

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::buffer(uint8_t num)
{
	bufferQueue.push('0' + (num/100));
	num %= 100;
	bufferQueue.push('0' + (num/10));
	num %= 10;
	bufferQueue.push('0' + num);
}

template<LogLevel OutputLevel> template<LogLevel Level> void MessageLog<OutputLevel>::bufferMessageStart()
{
	if (Level >= LogLevel::Debug)
	{
		buffer("D: ");
	}
	else if (Level == LogLevel::Info)
	{
		buffer("I: ");
	}
	else if (Level == LogLevel::Warning)
	{
		buffer("W: ");
	}
	else if (Level == LogLevel::Error)
	{
		buffer("E: ");
	}
	else if (Level == LogLevel::Fatal)
	{
		buffer("F: ");
	}
}

template<LogLevel OutputLevel> void MessageLog<OutputLevel>::bufferMessageEnd()
{
	bufferQueue.push('\r');
	bufferQueue.push('\n');
}


//public method implementations:
template<LogLevel OutputLevel> MessageLog<OutputLevel>::MessageLog(uint8_t bufferSize) : bufferQueue(bufferSize), writer(nullptr)
{
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
