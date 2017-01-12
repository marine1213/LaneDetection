/*
 * PosEvaluationThread.h
 *
 *  Created on: Jan 5, 2017
 *      Author: gnouchnam
 */

#ifndef SRC_THREADS_POSEVALUATIONTHREAD_H_
#define SRC_THREADS_POSEVALUATIONTHREAD_H_

#include "../System/SubThread.h"
#include "../Controller/CarCtrl.h"

class PosEvaluationThread : public SubThread{
public:
	PosEvaluationThread(CarCtrl *ctl){
		carCtl = ctl;
	};
	void startThread(){
		this->exec();
	}
private:
	CarCtrl *carCtl;

	void content(){
		carCtl->findMyPosition();
	}
};



#endif /* SRC_THREADS_POSEVALUATIONTHREAD_H_ */
