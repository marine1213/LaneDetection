/*
 * CarCtrl.cpp
 *
 *  Created on: Oct 28, 2016
 *      Author: gnouchnam
 */


#include "CarCtrl.h"

void *goScenario(void* data) {
	cout<<"scenario"<<endl;
	CarCtrl *ctrl = (CarCtrl *)data;
	string listCmd[5] = { "s", "8", "2", "2","2" };
	int waitTime[5] = { 300000, 6500000, 100000,100000,0 };

	for (int i = 0; i < 5 && ctrl->isWorking; ++i) {
			ctrl->getSerialCtrl()->writeData(listCmd[i].c_str());
			if (i == 1)
				ctrl->isRunning = true;
			usleep(waitTime[i]);
	};
	ctrl->isRunning = false;
	pthread_exit(NULL);
}
