/*!
 * This headerfile is µC-independent and can be used directly with all AVR's.
 */

#ifndef ATMIGHTY_INTERFACES_LISTENER_H_
#define ATMIGHTY_INTERFACES_LISTENER_H_


///This Interface represents an object which listens to an event which can be triggered externally
class Listener
{
	public:
		///virtual Destructor
		virtual ~Listener() {}

		///The callback function which gets called when the awaited event happens
		virtual void trigger() = 0;
};


#endif /* ATMIGHTY_INTERFACES_LISTENER_H_ */
