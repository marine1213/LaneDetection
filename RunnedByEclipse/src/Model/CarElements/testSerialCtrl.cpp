/*
 * testSerialCtrl.c
 *
 *  Created on: Oct 26, 2016
 *      Author: gnouchnam
 */


#include "../Controller/SerialCtrl.h"
#include <stdlib.h>

int main(){
	SerialCtrl ctrl = SerialCtrl();
	if(ctrl.getRfAbility()){
		cout<<("Setup serial succeed\n");
		char *cmd = new char[30];
		memset(cmd,0,sizeof(cmd));
		while(true){
			cin>>cmd;
			ctrl.writeData(cmd);
			cout<<"Sent:"<<cmd<<endl;
			if(cmd[0]=='0')
				break;
//			usleep(500000);
//			ctrl.writeData("s");
		}
		if (cmd) {
			free (cmd);
			cmd = 0;
		}
	}
	else
		cout<<("Setup serial failed");
	return 0;
}

