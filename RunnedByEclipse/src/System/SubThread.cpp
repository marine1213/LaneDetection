/*
 * SubThread.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#include "SubThread.h"


void *subFunction(void *data){
	SubThread *subThreadPtr = (SubThread*)data;
	while(subThreadPtr->isRunning()){
		if(subThreadPtr->isLockedData()){
			if(!subThreadPtr->isRunning())
				break;
			usleep(1000);
			continue;
		}
		subThreadPtr->content();
		subThreadPtr->setLockData(true);
	}
	pthread_exit(NULL);
}


