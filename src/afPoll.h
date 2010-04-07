/*
 * afPoll.h
 *
 *  Created on: Mar 17, 2010
 *      Author: darko
 */

#ifndef AFPOLL_H_
#define AFPOLL_H_

#include <avahi-common/watch.h>
#include <avahi-common/error.h>

/**
 * base class for avahi poll
 * possible polls are:
 *		- a simple poll
 *		- a threaded poll which when dispatched goes into a separated poll
 *		- a poll which is integrated with the qt main loop
 */
class afPoll {
public:
	afPoll();
	virtual ~afPoll();

	/** run the loop */
	virtual void dispatch() = 0;
	/** exit the loop */
	virtual void stop() = 0;
	/** return the correspondent avahi poll */
	virtual const AvahiPoll* getAvahiPoll() = 0;

};

#endif /* AFPOLL_H_ */
