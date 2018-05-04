/*!
 * This file implements all the classes from the MessageLogWriter namespace
 */


#include "MessageLogWriter.h"


using namespace MessageLogWriter;

//Base
void Base::trigger()
{
	while(!queue->isEmpty())
	{
		queue->pop();
	}
}
