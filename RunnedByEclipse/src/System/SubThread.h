/*
 * SubThread.h
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_CONTROLLER_SUBTHREAD_H_
#define SRC_CONTROLLER_SUBTHREAD_H_

#include <unistd.h> /* UNIX Standard Definitions         */
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

using namespace std;



void *subFunction(void *data);

class SubThread{


private:
	bool lockedData, running;
	pthread_t *thread;
public:

	SubThread(){
		lockedData 	= true;
		running		= false;
		thread		= (pthread_t*)malloc(sizeof(pthread_t));
	}

	virtual ~SubThread(){
		delete thread;
	}

protected:
	void exec(){
		lockedData = false;
		if(!running){
			running = true;
			pthread_create(thread,NULL,subFunction, this);
		}
	}

public:

	virtual void content(){

	}

	bool isLockedData() const {
		return lockedData;
	}

	void setLockData(bool lockData) {
		this->lockedData = lockData;
	}

	bool isRunning() const {
		return running;
	}

	/**
	 * should not be called anywhere else outside of subthread.cpp
	 */
	void stopThread(){
		running = false;
		pthread_join(*thread,NULL);
	}
};



#endif /* SRC_CONTROLLER_SUBTHREAD_H_ */
