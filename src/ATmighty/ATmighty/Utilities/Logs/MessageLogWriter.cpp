/*!
 * This file implements all the classes from the MessageLogWriter namespace
 */


#include "MessageLogWriter.h"

#include <stdint.h>
#include <stdlib.h>
#include "ATmighty/DataStructures/IoQueue.h"

//temp
#include "ATmighty/Ressources/Periphery/Physical/Usart.h"
#include <ATmighty/Ressources/Periphery/Physical/PhysicalHardwareManager.h>
#include "ATmighty/Utilities/LUTs/HardwareOwnerID.h"


using namespace MessageLogWriter;


//Base
Base::~Base()
{
	queue->setInitialPushListener(nullptr);
}
void Base::init(IoQueue<char>* logBuffer)
{
	queue = logBuffer;
	logBuffer->setInitialPushListener(this);
}


//Usart
Usart::~Usart()
{
	if (usart)
	{
		PhysicalHardwareManager::Free<Usart0>(&usart);
	}
}
void Usart::init(IoQueue<char>* logBuffer)
{
	//allocate necessary hardware
	usart = PhysicalHardwareManager::Alloc<Usart0>(OwnerID::MsgLogWriter);

	//Serial init
	usart->setUBRR0H(0x0F & (uint8_t)(103>>8));
	usart->setUBRR0L((uint8_t)(103));
	usart->setUCSR0C((0<<4)|(((1-1)&0x1)<<3)|(((8-5)&0x3)<<1));
	usart->setUCSR0B(usart->getUCSR0B() | (1<<TXEN0));  //enable Transmit-Unit

	Base::init(logBuffer);
}
void Usart::exit()
{
	//Serial finish (implement later)

	queue->setInitialPushListener(nullptr);

	//free the used hardware
	PhysicalHardwareManager::Free<Usart0>(&usart);
}
void Usart::trigger()
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
