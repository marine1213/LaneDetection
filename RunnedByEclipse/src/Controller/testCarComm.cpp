/*
 * testCarCtrl.c
 *
 *  Created on: Oct 27, 2016
 *      Author: gnouchnam
 */


#include <stdlib.h>
#include "../Controller/CarComm.h"


int subMain(){
	CarComm carCtl = CarComm();
	while(true){
		char c;
		cin >> c;
		if (c == '0')
			break;
		carCtl.go();
	}
	return 0;
}

