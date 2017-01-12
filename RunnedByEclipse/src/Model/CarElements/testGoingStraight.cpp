/*
 * testGoingStraight.cpp
 *
 *  Created on: Oct 29, 2016
 *      Author: gnouchnam
 */



#include <stdlib.h>

#include "../../Controller/CarComm.h"
#define SIZE	6

void testGoingStraightLeft(SerialCtrl *ctrl){
	char cmd[SIZE][2] = {"s","8","4","6","5","2"};
	int time[SIZE] = {300000,800000,200000,357500,200000,0};
	for (int i = 0; i < SIZE; ++i) {
		ctrl->writeData(cmd[i]);
		usleep(time[i]);
	}
}

void testGoingStraightRight(SerialCtrl *ctrl){
	char cmd[SIZE][2] = {"s","8","6","4","5","2"};
	int time[SIZE] = {300000,800000,340000,200000,200000,0};
	for (int i = 0; i < SIZE; ++i) {
		ctrl->writeData(cmd[i]);
		usleep(time[i]);
	}
}

int main(){
	CarComm carCtl = CarComm();
	while(true){
		char c;
		cin >> c;
		if (c == '0')
			break;
//		testGoingStraightRight(carCtl.getSerialCtrl());
	}
	return 0;
}


