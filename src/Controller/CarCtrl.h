/*
 * CarCtrl.h
 *
 *  Created on: Oct 26, 2016
 *      Author: gnouchnam
 */

#ifndef CARCTRL_H_
#define CARCTRL_H_

#include "SerialCtrl.h"
#include "string.h"
#include <pthread.h>

void *scenario(void* data);

class CarCtrl {
private:
	SerialCtrl *ctrl;
	pthread_t workerThread;

	int	previousState;

public:
	bool	isRunning;

	CarCtrl(){
		cout<<"car ctrl constructor"<<endl;
		ctrl = new SerialCtrl();
		isRunning = false;
		previousState = 0;
	}

	~CarCtrl(){
		//pthread_cancel(*workerThread);
		delete ctrl;
	}

	void go(){
		pthread_create(&workerThread,NULL,scenario, this);
	}

	void turnLeft(){
		if(isRunning){// && previousState != -1
			ctrl->writeData("4");
		}
	}

	void turnRight(){
		if(isRunning ){//&& previousState != 1
			ctrl->writeData("6");
		}
	}

	void goStraight(){
		if(isRunning ){//&& previousState != 0
			ctrl->writeData("5");
			previousState = 0;
			usleep(30);
		}
	}

	SerialCtrl* getSerialCtrl() {
		return ctrl;
	}
};

#endif /* CARCTRL_H_ */
