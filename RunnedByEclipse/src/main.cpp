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
  waitKey(0);
  exit(1);
}


int main(int argc, char** args){
	signal(SIGSEGV, handler);
	signal(SIGABRT, handler);

//	testKalman();
	DataBundle data = DataBundle();
	CarCtrl	carCtrl = CarCtrl(&data);
	main_imgProcessing(argc,args,data, carCtrl);
	return 1;
}
