/*
 * testCarCtrl.c
 *
 *  Created on: Oct 27, 2016
 *      Author: gnouchnam
 */


#include "CarCtrl.h"
#include <stdlib.h>


int main(){
	CarCtrl carCtl = CarCtrl();
	while(true){
		char c;
		cin >> c;
		if (c == '0')
			break;
		carCtl.go();
	}
	return 0;
}

