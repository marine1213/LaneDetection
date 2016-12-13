#include "main.h"


// trace back error
void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

// control function which is used to send trigger message to the car
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
	signal(SIGSEGV, handler);
	signal(SIGABRT, handler);

	Data data = Data();
	pthread_t controlThread;
	pthread_create(&controlThread,NULL,controlFunc, &data);
	main_imgProcessing(argc,args,data);
	return 1;
}
