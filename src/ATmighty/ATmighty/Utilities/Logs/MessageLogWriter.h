/*
 * This headerfile declares different MessageLogWriter classes as well as their common base class.
 */

#ifndef ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_
#define ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_


#include <stdint.h>
#include <stdlib.h>
#include "ATmighty/DataStructures/IoQueue.h"
#include "ATmighty/Interfaces/Listener.h"
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"

//temp
#include <ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h>


/*
 * This namespace collects different MessageLogWriter classes as well as their common base class.
 */
namespace MessageLogWriter
{
	/*!
	 * This class represents a dummy MessageLogWriter which does nothing and is used as a common base-class for all the other MessageLogWriter classes.
	 * Derive from this and override function write() respectively to declare a new MessageLogWriter.
	 */
	class Base : public Listener
	{
		protected:
			///A reference to the IoQueue which is the internal buffer of the corresponding MessageLog
			IoQueue<char>* queue;

		public:
			///The virtual Destructor
			virtual ~Base() {queue->setInitialPushListener(nullptr);}

			///The callback function which gets called as soon as a new message arrives in the buffer.
			virtual void trigger();

			///Start writing from the given IoQueue-buffer
			virtual void init(IoQueue<char>* logBuffer)
			{
				queue = logBuffer;
				logBuffer->setInitialPushListener(this);
			}
	};

	/*!
	 * This class represents a MessageLogWriter which prints out the Messages via Usart/(USB)
	 */
	//temp - change Implementation later to use abstract hardware manager + move to cpp-file
	class Usart : public Base
	{
		private:
			Usart0* usart;

		public:
			///Constructor
			Usart()
			{
				usart = PhysicalHardwareManager::Alloc<Usart0>(100);
			}

			///Destructor
			~Usart()
			{
				PhysicalHardwareManager::Free<Usart0>(&usart);
			}

			virtual void init(IoQueue<char>* logBuffer)
			{
				//Serial init
				usart->setUBRR0H(0x0F & (uint8_t)(103>>8));
				usart->setUBRR0L((uint8_t)(103));
				usart->setUCSR0C((0<<4)|(((1-1)&0x1)<<3)|(((8-5)&0x3)<<1));
				usart->setUCSR0B(usart->getUCSR0B() | (1<<TXEN0));  //enable Transmit-Unit

				Base::init(logBuffer);
			}

			virtual void trigger()
			{
				while(!queue->isEmpty())
				{
					while (!(usart->getUCSR0A() & _BV(UDRE0)))
					{
						asm("nop");
					}
					usart->setUDR0(queue->pop());
				}
			}
	};
}


#endif /* ATMIGHTY_UTILITIES_LOGS_MESSAGELOGWRITER_H_ */
