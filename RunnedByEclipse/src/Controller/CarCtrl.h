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
#include <stdlib.h>
#include <pthread.h>

void *goScenario(void* data);

class CarCtrl {
private:
	SerialCtrl *ctrl;
	pthread_t *goingThread, *shockSpeedThread;


	int	previousState;
	// to be shown
	char *eventMsg;

public:
	bool	isRunning;
	bool 	isWorking;
	bool	isSudden;
	// to be shown
	float dStop;

	CarCtrl(){
		cout<<"car ctrl constructor"<<endl;
		ctrl = new SerialCtrl();

		ctrl->writeData("c410");
		ctrl->writeData("4");

		goingThread = (pthread_t*)malloc(sizeof(pthread_t));
		shockSpeedThread = (pthread_t*)malloc(sizeof(pthread_t));

		isWorking = isRunning = false;
		previousState 	= 0;
		dStop 			= 0;
		eventMsg 		= NULL;
		isSudden		= true;
	}

	~CarCtrl(){
		cout<<"Car control Destructor"<<endl;
		//pthread_cancel(*workerThread);
		delete ctrl;
		delete goingThread;
		delete shockSpeedThread;
	}

	void go(){
		isWorking = true;
		pthread_create(goingThread,NULL,goScenario, this);
	}

	void moveLittleLeft(){
		if(isRunning){// && previousState != -1
			ctrl->writeData("4");
		}
	}

	void turnStraight(){
		if(isRunning){// && previousState != -1
			ctrl->writeData("5");
		}
	}

	void turnLeft(){
		if(isRunning){// && previousState != -1
			ctrl->writeData("7");
		}
	}

	void turnRight(){
		if(isRunning ){//&& previousState != 1
			ctrl->writeData("9");
		}
	}

	void moveLittleRight(){
		if(isRunning ){//&& previousState != 1
			ctrl->writeData("6");
		}
	}

	void stop(){
		if(isRunning ){//&& previousState != 1
			ctrl->writeData("2");
		}
	}

	void setTurnLeftAngle(){
		ctrl->writeData("c715");
	}

	void setTurnRightAngle(){
		ctrl->writeData("c730");
	}

	void haltCurrentWork(){
		isWorking = false;
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

	void setEventMsg(const char *str){
		int len = strlen(str);
		if(eventMsg)
			delete eventMsg;
		eventMsg = (char*)malloc(len * sizeof(char));
		memcpy(eventMsg,str,len);
	}

	char* getEventMsg() const{
		if(!eventMsg)
			return "";
		return eventMsg;
	}
};

#endif /* CARCTRL_H_ */
