/*
 * This headerfile is µC-independent and can be used directly with all AVR's.
 * This headerfile declares different MessageLogWriter classes as well as their common base class.
 */

#ifndef ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_
#define ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_


#include "ATmighty/Interfaces/Listener.h"


template<class T> class IoQueue;
class Usart0;


/*
 * This namespace collects different MessageLogWriter classes as well as their common base class.
 * Each MessageLogWriter instance can only be used with one MessageLog.
 */
namespace MessageLogWriter
{
	/*!
	 * This class represents a dummy MessageLogWriter which is used as a common base-class for all the other MessageLogWriter classes.
	 * Derive from this and implement function trigger() (from derived Listener-class) respectively to declare a new MessageLogWriter.
	 */
	class Base : public Listener
	{
		protected:
			///A reference to the IoQueue which is the internal buffer of the corresponding MessageLog
			IoQueue<char>* queue;

		public:
			///The virtual Destructor
			virtual ~Base();

			///Start writing from the given IoQueue-buffer -> allocate and configurate necessary hardware
			virtual void init(IoQueue<char>* logBuffer);

			///Stop writing -> free necessary hardware
			virtual void exit() = 0;
	};

	/*!
	 * This class represents a MessageLogWriter which prints out the Messages via Usart/(USB)
	 */
	//TODO change Implementation later to use abstract hardware manager
	class Usart : public Base
	{
		private:
			Usart0* usart;

		public:
			///Destructor
			virtual ~Usart();

			///Initializes the necessary hardware for writing out messages. Gets called when hooking up to a MessageLog instance with calling setWriter(*this).
			virtual void init(IoQueue<char>* logBuffer);

			///Frees the hardware used to write out messages. Gets called when removing the connection to a MessageLog insatnce with calling setWriter(otherMessageLogWriter).
			virtual void exit();

			virtual void trigger();
	};
}


#endif /* ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_ */
