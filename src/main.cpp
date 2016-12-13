#include "main.h"

void *controlFunc(void *voidData){
	Data* data = (Data*)voidData;
	while (true) {
		char c;
		cin >> c;
		if (c == '0')
			break;
		data->carCtl->go();
	}
	pthread_exit(NULL);
}

int main(int argc, char** args){
	Data data = Data();
	pthread_t controlThread;
	pthread_create(&controlThread,NULL,controlFunc, &data);
	main_imgProcessing(argc,args,data);
}
