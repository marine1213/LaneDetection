/*
 * DriveThread.h
 *
 *  Created on: Jan 5, 2017
 *      Author: gnouchnam
 */

#ifndef SRC_THREADS_DRIVETHREAD_H_
#define SRC_THREADS_DRIVETHREAD_H_


#include "../constant.h"
#include "../System/SubThread.h"

class DriveThread : public SubThread{
public:
	DriveThread(CarComm *sourceComm){
		event = Constant::STOP_EVENT;
		carComm = sourceComm;
		sent = false;
	};

	void input(Constant::ControlEvent newEvent){
		sent = false;
		event = newEvent;
		this->exec();
	}

	bool isSent(){
		return sent;
	}
private:
	CarComm *carComm;
	Constant::ControlEvent event;
	bool	sent;

	void content(){
		switch (event) {
		case Constant::TURN_LEFT_EVENT:
			carComm->turnLeft();
			sent = true;
			break;
		case Constant::TURN_RIGHT_EVENT:
			carComm->turnRight();
			sent = true;
			break;
		case Constant::BALANCE_LEFT_EVENT:
			carComm->moveLittleRight();
			sent = true;
			break;
		case Constant::BALANCE_RIGHT_EVENT:
			carComm->moveLittleLeft();
			sent = true;
			break;
		case Constant::STOP_EVENT:
			carComm->stop();
			sent = true;
			break;
		case Constant::START_EVENT:
			carComm->start();
			sent = true;
			break;
		default:
			// not considered action
			break;
		}
	}
};



#endif /* SRC_THREADS_DRIVETHREAD_H_ */
