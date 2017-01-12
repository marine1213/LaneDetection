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
#include "../System/SubThread.h"

void *goScenario(void* data);

class CarComm : public SubThread{

private:
	SerialCtrl *ctrl;

	bool 		movingPeriod;

	// will be run when exec() is called
	void content(){
		cout<<"scenario"<<endl;
		string listCmd[1] = { "s" };
		int waitTime[1] = { 6800000};

		for (int i = 0; i < 1 && isRunning(); ++i) {
			ctrl->writeData(listCmd[i].c_str());
			if(i == 1)
				movingPeriod = true;
			usleep(waitTime[i]);
		};
		movingPeriod = false;
	}

public:

	CarComm(){
#if	SHOW_CLASSES_STATE
		cout<<"car ctrl constructor"<<endl;
#endif
		ctrl = new SerialCtrl();

		ctrl->writeData("c410");
		usleep(50000);
		ctrl->writeData("c728");
		usleep(50000);
		ctrl->writeData("4");

		movingPeriod	= false;
	}

	~CarComm(){
#if SHOW_CLASSES_STATE
		cout<<"Car control Destructor"<<endl;
#endif
		delete ctrl;
	}

	void go(){
		this->exec();
	}

	void start(){
		ctrl->writeData("s");
	}

	void moveLittleLeft(){
		ctrl->writeData("4");
	}

	void turnStraight(){
		ctrl->writeData("5");
	}

	void turnLeft(){
		ctrl->writeData("7");
	}

	void turnRight(){
		ctrl->writeData("9");
	}

	void moveLittleRight(){
		ctrl->writeData("6");
	}

//	bool isMovingPeriod(){
//		return movingPeriod;
//	}

	void stopMovingPeriod(){
		movingPeriod = false;
	}

	void stop(){
		ctrl->writeData("2");
	}

	void setTurnLeftAngle(){
		ctrl->writeData("c730"); // #editHere c715
	}

	void setTurnRightAngle(){
		ctrl->writeData("c730");
	}

	void goStraight(){
		ctrl->writeData("5");
	}

};

#endif /* CARCTRL_H_ */
