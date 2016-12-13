#ifndef _MAIN_H_
#define _MAIN_H_

#include "common.h"
#include "Model/DataStructure.h"
#include "Controller/CarCtrl.h"
#include "img_processing.h"
#include "main_imgProcessingLib.h"

void main_imgProcessing(int argc, char** args,Data &data);



void main_imgProcessing(int argc, char** args,Data &data){
	// assertions
	assert((VIDEO + CAMERA + STATIC_IMAGE) == 1 && "Only one type of input should be enable at a time");
	init(argc,args);

	int frCount = 0;
	float fps = 0;
	bool isFirsFrm = true;
	int destBorderId = BORDER_TOP, startBorderId = BORDER_BOTTOM;

	char c = 'a', d = 1;
	Mat img, map;
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
		img = readImage(capture);
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
		Mat mvoOutput;
		if(isFirsFrm){
//			perspectiveTransform(img,map);
			setupPPMap(img,data.transformMat,map);
			cout<<"setup perspective map completed"<<endl;
			data.tmpDetectionImg = Mat::zeros(map.size(), CV_8UC3);
			imshow("map",map);
			data.setupIntersection(map.cols,map.rows);
			cout<<"setup size for intersection completed"<<endl;
			laneProcessing(map, d, data);
			cout<<"Finding lane completed"<<endl;
			data.intersection->setupCenterArea();
			cout<<"setup intersectionc completed"<<endl;
			isFirsFrm = false;
		}

		//imwrite("intersection.jpg",img);
		mvoFilter(img, mvoOutput, data);

		data.showIntersection(data.tmpDetectionImg);
		data.showInterpolatedLineData(data.tmpDetectionImg);

		Mat tempImg = data.tmpDetectionImg.clone();
		Mat mapCln = map.clone();
		LinesConnector *ipl = data.intersection->createPath(startBorderId, destBorderId);
		drawConnectorLines(ipl, tempImg);
		Scalar black = Scalar(0,0,0);
		drawConnectorLines(ipl, mapCln, &black);

		delete ipl;

		char nextId = cvWaitKey(1);
		if(nextId == 'c'){
			destBorderId = (destBorderId+1)%4;
			if(destBorderId == startBorderId)
				destBorderId = (destBorderId+1)%4;
		}
		if(nextId == 'd'){
			startBorderId = (startBorderId+1)%4;
			if(startBorderId == destBorderId)
				startBorderId = (startBorderId+1)%4;
		}

		data.showCar(tempImg);
		data.showCar(mapCln);

		data.overLaneAssessment(destBorderId);
		if(data.getCarObj()){
			drawPointsBuffer(data.getCarObj()->getBuff100Pts(), tempImg);
		}
//		data.clearCarData();

		imshow("express lanes", tempImg);
		ostringstream ss,ss1,ss2,ss3;
		ss <<"FPS="<<fps <<" - D="<<data.carCtl->dStop<<" - "<<data.carCtl->getEventMsg();
		putText(mapCln, ss.str(), Point(30, 30), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		ss1 <<"BorderID:"<<data.getCarObj()->getPath().getCurrentBorderId();
		ss1 <<" - LaneID:"<<data.getCarObj()->getLaneId();
		putText(mapCln, ss1.str(), Point(30, 60), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		ss2 <<"Direction:"<<data.getCarObj()->getPath().getDirectionArg()<<" - " <<"Using ipl:"<<data.getCarObj()->isUsingIpl();
		putText(mapCln, ss2.str(), Point(30, 90), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		ss3 <<"Car postion on track:"<< data.getCarObj()->getPath().getCarPosState();
		putText(mapCln, ss3.str(), Point(30, 120), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
		imshow("OUTPUT", mapCln);

		//d++;

		c = waitKey(WAIT_TIME);
	}
	onExit();
}

#endif // !_MAIN_H_
