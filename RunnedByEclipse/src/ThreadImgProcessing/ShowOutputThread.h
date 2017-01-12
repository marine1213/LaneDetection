/*
 * ShowOutputThread.h
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_THREADIMGPROCESSING_SHOWOUTPUTTHREAD_H_
#define SRC_THREADIMGPROCESSING_SHOWOUTPUTTHREAD_H_

#include "../common.h"
#include "../System/SubThread.h"

class ShowOutputThread : public SubThread{
public:
	ShowOutputThread(){
		map = NULL;
		data = NULL;
		fps = -1;
		startBorderId = -1;
		destBorderId = -1;
//		nextId = -1;
	}

	void input(Mat *map, float fps, char charKey, DataBundle *data){
		this->map 		= map;
		this->data		= data;
//		this->nextId	= charKey;
		this->fps		= fps;
		this->exec();
	}
private:

	Mat *map;
	DataBundle *data;
	int startBorderId, destBorderId;
	float fps;
//	char nextId;

	void content(){
		Mat tempImg = data->getTmpDetectionImg()->clone();
		Mat mapCln = map->clone();
//		LinesConnector *ipl = data->intersection->createPath(startBorderId, destBorderId);
//		drawConnectorLines(ipl, tempImg);
//		Scalar black = Scalar(0,0,0);
//		drawConnectorLines(ipl, mapCln, &black);
//
//		delete ipl;
////
//		if(nextId == 'c'){
//			destBorderId = (destBorderId+1)%4;
//			if(destBorderId == startBorderId)
//				destBorderId = (destBorderId+1)%4;
//		}
//		if(nextId == 'd'){
//			startBorderId = (startBorderId+1)%4;
//			if(startBorderId == destBorderId)
//				startBorderId = (startBorderId+1)%4;
//		}

		data->showCar(tempImg);
		data->showCar(mapCln);

		if(data->getCarData()){
			drawPointsBuffer(data->getCarData()->getBuff50Pts(), tempImg);
			drawPointsBuffer(data->getCarData()->getBuff50Kal(), tempImg, Scalar(255,0,0));
		}
//		data->clearCarData();

		imshow("express lanes", tempImg);
		ostringstream ss,ss1,ss2,ss3;
		ss <<"FPS="<<fps <<" - D=";
		putText(mapCln, ss.str(), Point(30, 30), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//		if(data->getCarObj()){
//			ss1 <<"BorderID:"<<data->getCarObj()->getPath().getCurrentBorderId();
//			ss1 <<" - LaneID:"<<data->getCarObj()->getLaneId();
//			putText(mapCln, ss1.str(), Point(30, 60), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//			ss2 <<"Direction:"<<data->getCarObj()->getPath().getDirectionArg()<<" - ";
//			putText(mapCln, ss2.str(), Point(30, 90), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//			ss3 <<"Car postion on track:"<< data->getCarObj()->getPath().getCarPathState();
//			putText(mapCln, ss3.str(), Point(30, 120), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
//		}
		imshow("OUTPUT", mapCln);

	}
};



#endif /* SRC_THREADIMGPROCESSING_SHOWOUTPUTTHREAD_H_ */
