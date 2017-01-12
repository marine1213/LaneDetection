/*
 * mvoDetectionThread.h
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_THREADIMGPROCESSING_MVODETECTIONTHREAD_H_
#define SRC_THREADIMGPROCESSING_MVODETECTIONTHREAD_H_


#include "../common.h"
#include "../System/SubThread.h"
#include "../cameraFrameWork.h"
#include "../Threads/ThreadsManager.h"

//void mvoFilter(Mat &input, Mat &output,DataBundle &data);

class MvoDetectionThread : public SubThread{

public:
public:
	MvoDetectionThread(ThreadsManager &sourceManager){
		manager		= &sourceManager;
		data 		= manager->getCarCtrl()->getData();
		startTime 	= clock();
		this->elm = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));
	};

	void startThread(){
		this->exec();
	}

private:
	Mat elm;
	DataBundle *data;
	ThreadsManager *manager;

	clock_t startTime;

	void mvoDetection(Mat &img,DataBundle &outputData, float time);

	void content(){
		if(!data || data->getInput().empty())
			return;

		float sec = (float)(clock() - startTime)/CLOCKS_PER_SEC;
		// pre processing
		Mat input = data->getInput().clone();
		Mat grayInput;
		Mat cloneOutput;

		cvtColor(input, grayInput, COLOR_BGR2GRAY);
		(*data).getPMog2()->apply(grayInput, cloneOutput,0.0001);
		if(cloneOutput.depth() != CV_8U){
			cout<<"depth color error"<<endl;
			return;
		}
		morphologyEx(cloneOutput, cloneOutput, MORPH_CLOSE, elm);

		imshow("movObj", cloneOutput);
//		cout<<"mov obj"<<endl;

		// mvo detection
		mvoDetection(cloneOutput,*data,sec);
	}

};

#endif /* SRC_THREADIMGPROCESSING_MVODETECTIONTHREAD_H_ */
