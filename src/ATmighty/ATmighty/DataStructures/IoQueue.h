/*!
 * This headerfile is µC-independent and can be used directly with all AVR's.
 * This headerfile also contains the whole implementation of the IoQueue, as this is needed in order to implicitly instantiate a template class.
 */

#ifndef ATMIGHTY_DATASTRUCTURES_IOQUEUE_H_
#define ATMIGHTY_DATASTRUCTURES_IOQUEUE_H_


#include <util/atomic.h>
#include <stdint.h>

/*!
 * This class represents an Array-based queue with fixed size (2-255).
 * It doesn't use dynamic memory allocation, but may loose data if Queue is full.
 * The type of the data-elements to be stored is declared by the first template argument.
 * The size of the queue is defined by the second template argument. (which must be an uint8_t integer >= 1)
 * The size of this IoQueue in RAM will be size*sizeof(T)+4 Bytes.
 */
template <class T, uint8_t size> class IoQueue
{
	private:
		//The Index of the next element to pop
		uint8_t popIndex;

		//the number of elements currently stored in this IoQueue
		uint8_t count;

		//The Array which holds the actual data-elements
		T queue[size];

		//The optional pointer to a callback function which gets called when an new element is inserted into the empty IoQueue
		void(*listener)(void);


	public:
		/*!
		 * Default Constructor.
		 * The type of the data-elements to be stored in this IoQueue is declared by the first template argument.
		 * The size of the queue is defined by the second template argument. (which must be an uint8_t integer > 1)
		 */
		IoQueue() : popIndex(0), count(0), listener(0) {}

		///Returns true if count==0
		inline bool isEmpty() {return !count;}

		///Returns count (the number of elements currently stored in this IoQueue)
		inline uint8_t getSize() {return count;}

		/*!
		 * Inserts a new element into this IoQueue
		 * \param element The new element of type T (specified by the first template argument) to insert into this IoQueue
		 * \returns True when insertion is sucessfull, False when IoQueue is already full
		 */
		bool push(T element)
		{
			bool callback = false; //flag is set if listener must be called after push
			bool sucess = true; //flag buffers return-value

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (count < size) //queue not full?
				{
					uint8_t pushIndex = popIndex + count; //calculate pushIndex
					if (pushIndex >= size || pushIndex < popIndex) //overflow >= size or overflow > 255?
					{
						pushIndex += (0xFF - size); //correct pushIndex by offset 255-size
					}

					queue[pushIndex] = element; //insert element
					if (count == 0) //first element in empty queue?
					{
						callback = true;
					}
					count++; //increment element-counter
				}
				else
				{
					sucess = false;
				}
			}

			if (callback && listener) //must call listener?
			{
				listener();
			}

			return sucess;
		}

		///Removes the next element from this IoQueue and returns it.
		T pop()
		{
			T buf;

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (count)
				{
					buf = queue[popIndex];
					count--;
					popIndex++;
					if (popIndex >= size)
					{
						popIndex = 0;
					}
				}
				else //empty
				{
					buf = 0;
				}
			}

			return buf;
		}

		///Returns the next element from this IoQueue without removing it.
		T peek()
		{
			T buf;

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (count)
				{
					buf = queue[popIndex];
				}
				else
				{
					buf = 0;
				}
			}

			return buf;
		}

		///Empty the IoQueue. All currently stored elements will be lost.
		inline void clear() {count = 0;}

		///Sets a function-pointer which gets called when a new element is inserted into a empty IoQueue. This is useful to start a routine which empties the IoQueue
		void setInitialPushListener(void (*listener)(void))
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				this->listener = listener;
			}
		}
};


#endif /* ATMIGHTY_DATASTRUCTURES_IOQUEUE_H_ */
