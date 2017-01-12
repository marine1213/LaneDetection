#ifndef _MAIN_H_
#define _MAIN_H_

#include "cameraFrameWork.h"
#include "common.h"
#include "Controller/CarCtrl.h"
#include "ThreadImgProcessing/MvoDetectionThread.h"
#include "ThreadImgProcessing/ShowOutputThread.h"
#include "ThreadImgProcessing/img_processing.h"

void main_imgProcessing(int argc, char** args,DataBundle &data, CarCtrl &carCtrl);



void main_imgProcessing(int argc, char** args,DataBundle &data, CarCtrl &carCtrl){
	// assertions
	assert((VIDEO + CAMERA + STATIC_IMAGE) == 1 && "Only one type of input should be enable at a time");
	init(argc,args,data);

	int destBorderId = BORDER_LEFT;//, startBorderId = BORDER_BOTTOM;

	int frCount = 0;
	float fps = 0;
	bool isFirsFrm = true;

	char c = 'a', d = 1;
	Mat img, map;

	carCtrl.getPath().setDestinationBorder(destBorderId);
	ThreadsManager		threadManager(&carCtrl);
	MvoDetectionThread 	mvoDectectionThread(threadManager);
//	ShowOutputThread	showOutputThread;

	clock_t t = clock();
	while(c != 27){
#if STATIC_IMAGE
		if(!(ent = readdir (dir)))
			break;
		img = readImage(ent->d_name);
#if SHOW_INPUT
		if(!img.empty()) // check if file is directory or not image
			imshow(ent->d_name, img);
#endif //SHOW_INPUT
#elif VIDEO || CAMERA
		img = readImage();
		frCount++;
		clock_t delta = clock() - t;
		float sec = (float)delta/CLOCKS_PER_SEC;
		if( sec > 1){
			fps = frCount/sec;
			t = clock();
			frCount = 0;
		}
#if SHOW_INPUT
		if(img.empty())
			break;
		imshow("Preview", img);
#endif //SHOW_INPUT
#endif
		if(img.empty())
			continue;
		img.copyTo(data.getInput());

		if(isFirsFrm){
//			perspectiveTransform(img,map);
			// setup perspective area
			setupPPMap(img,data.getTransformMat(),map);
			cout<<"setup perspective map completed"<<endl;
			imshow("map",map);
			// create intersection model and temp detection image
			data.setupIntersection(map.cols,map.rows);
			cout<<"setup size for intersection completed"<<endl;
			// merge lane with intersection
			laneProcessing(map, d, data);
			cout<<"Finding lane completed"<<endl;
			// recognize center area
			data.getIntersection()->setupCenterArea();
			cout<<"setup intersection completed"<<endl;
			isFirsFrm = false;

			data.showIntersection(*data.getTmpDetectionImg());
			data.showInterpolatedLineData(*data.getTmpDetectionImg());
			//pixelPicker(data.tmpDetectionImg);

		}
		mvoDectectionThread.startThread();

		if(c=='s'){
			if(!carCtrl.getDriveThread()->isRunning()){
				carCtrl.getData()->getCarData()->clearBuffer();
				carCtrl.getDriveThread()->input(Constant::START_EVENT);
			}
		}

		Mat tempImg = data.getTmpDetectionImg()->clone();
		Mat mapCln = map.clone();

		data.showCar(tempImg);
		data.showCar(mapCln);

		if(data.getCarData()){
			drawPointsBuffer(data.getCarData()->getBuff50Pts(), tempImg);
			drawPointsBuffer(data.getCarData()->getBuff50Kal(), tempImg, Scalar(255,0,0));
		}

		imshow("express lanes", tempImg);
		ostringstream ss,ss1,ss2,ss3;
		ss <<"FPS="<<fps;
		putText(mapCln, ss.str(), Point(30, 30), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		if(data.getCarData()){
//			ss1 <<"BorderID:"<<data.getCarObj()->getPath().getCurrentBorderId();
//			ss1 <<" - LaneID:"<<data.getCarObj()->getLaneId();
//			putText(mapCln, ss1.str(), Point(30, 60), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//			ss2 <<"Direction:"<<data.getCarObj()->getPath().getDirectionArg()<<" - ";
//			putText(mapCln, ss2.str(), Point(30, 90), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//			ss3 <<"Car postion on track:"<< data.getCarObj()->getPath().getCarPathState();
//			putText(mapCln, ss3.str(), Point(30, 120), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		}
		imshow("OUTPUT", mapCln);

		c = waitKey(WAIT_TIME);
	}

	// stop other sub threads
	mvoDectectionThread.stopThread();
	threadManager.stopAllThreads();
//	showOutputThread.stopThread();
	onExit();
}

#endif // !_MAIN_H_
