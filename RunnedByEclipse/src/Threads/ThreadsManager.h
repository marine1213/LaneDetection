/*
 * ThreadManager.h
 *
 *  Created on: Jan 6, 2017
 *      Author: gnouchnam
 */

#ifndef SRC_THREADS_THREADSMANAGER_H_
#define SRC_THREADS_THREADSMANAGER_H_

#include "DriveThread.h"
#include "EventDetectionThread.h"
#include "PosEvaluationThread.h"
#include "VelocityThread.h"

class ThreadsManager{
private:
	EventDetectionThread 	*eventDetectionThread;
	PosEvaluationThread 	*posEvaluationThread;
	VelocityThread			*velocityThread;

	CarCtrl	*carCtrl;
public:
	ThreadsManager(CarCtrl	*sourceCtrl){
		carCtrl					= sourceCtrl;
		eventDetectionThread 	= new EventDetectionThread(carCtrl);
		posEvaluationThread 	= new PosEvaluationThread(carCtrl);
		velocityThread			= new VelocityThread(carCtrl);
	}

	~ThreadsManager(){
		delete eventDetectionThread;
		delete posEvaluationThread;
		delete velocityThread;
	}

	void stopAllThreads(){
		eventDetectionThread->stopThread();
		posEvaluationThread->stopThread();
		velocityThread->stopThread();
	}

	void startAllThreads(){
		eventDetectionThread->startThread();
		posEvaluationThread->startThread();
		velocityThread->startThread();
	}
	VelocityThread* getVelocityThread(){
		return velocityThread;
	}
	EventDetectionThread* getEventDetectionThread(){
		return eventDetectionThread;
	}
	PosEvaluationThread* getPosEvaluationThread(){
		return posEvaluationThread;
	}
	CarCtrl* getCarCtrl(){
		return carCtrl;
	}
};



#endif /* SRC_THREADS_THREADSMANAGER_H_ */
