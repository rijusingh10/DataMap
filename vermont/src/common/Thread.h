/*
 * PSAMP Reference Implementation
 *
 * Thread.h
 *
 * Implementation of a base class for a thread
 *
 * Author: Michael Drueing <michael@drueing.de>
 *
 */

#ifndef THREAD_H
#define THREAD_H

#include "msg.h"

typedef void* (*ThreadFunc)(void *);

class Thread
{
	public:
		Thread(ThreadFunc threadFunction) : exitFlag(false), thread_created(false), f(threadFunction)
		{
		};

		void run(void *threadData)
		{
			// remember that run() has been called
			thread_created = true;

			//data = threadData;
			DPRINTF("creating new thread");
			if (pthread_create(&thread, NULL, f, threadData) != 0) {
				THROWEXCEPTION("failed to create new thread");
			}
		};

		void *join()
		{
			// do not attempt to join if run() had not yet been called
			if (!thread_created) return 0;

			void *result=NULL;
			if(!thread || pthread_join(thread, &result)) {
				msg(MSG_ERROR, "joining failed");
			}
			thread_created = false;

			return result;
		};

		bool detach()
		{
			// do not attempt to detach if run() had not yet been called
			if (!thread_created) return 0;

			bool result = (pthread_detach(thread) == 0);
			thread_created = false;

			return result;
		}

		~Thread()
		{
		};

		bool exitFlag;
		//void *data;

	private:
		pthread_t thread;
		bool thread_created; /**< true after Thread::run() was called */
		ThreadFunc f;
};

#endif
