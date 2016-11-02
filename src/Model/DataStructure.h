/*
 * DataStructure.h
 *
 *  Created on: Oct 25, 2016
 *      Author: gnouchnam
 */

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#include "../common.h"
#include "Line.h"
#include "Lane.h"
#include "Car.h"
#include "../draw_helper.h"

class Data{
public:
	vector<Lane*> lanes;
	vector<Line*> interpolatedLines;

	Ptr< BackgroundSubtractor> pMOG2;
	Car *car;
	CarCtrl *carCtl;

	Mat tempMat;

	Data(){
		pMOG2 	= createBackgroundSubtractorMOG2(500,16,false);
		car 	= NULL;
		carCtl 	= new CarCtrl();
	}

	void overLaneAssessment(){
		if(car){
			if(car->centerLineId == -1)
				car->findMyInterpolatedLines(interpolatedLines);
			if(car->centerLineId != -1){
//				car->distanceToLanes(lanes);
				car->distanceToInterpolatedLines(interpolatedLines, *carCtl);
			}
		}
	}

	void showLaneData(Mat &img){
		for (int i = 0; i < lanes.size(); ++i){
			ostringstream ss;
			ss << i;
			Line *temp = lanes[i]->getCenter();
			temp->setLimitedArea(0,0,img.cols,img.rows);
			putText(img, ss.str(), Point(temp->getPoint2()[0], temp->getPoint2()[1]),
					FONT_HERSHEY_COMPLEX, 1,Scalar(255, 255, 255), 1, CV_AA);
			drawLane(*(lanes[i]),img);
		}
	}

	void showInterpolatedLineData(Mat &img){
		for (int i = 0; i < interpolatedLines.size(); ++i){
			ostringstream ss;
			ss << i;
			interpolatedLines[i]->setLimitedArea(0,0,img.cols,img.rows);
			putText(img, ss.str(), Point(interpolatedLines[i]->getPoint2()[0], interpolatedLines[i]->getPoint2()[1]),
					FONT_HERSHEY_COMPLEX, 1,Scalar(255, 255, 255), 1, CV_AA);
			Scalar centerColor = Scalar(255,255,0);
			if(i%3 == 0)
				drawLine(*interpolatedLines[i],img,centerColor);
			else
				drawLine(*interpolatedLines[i],img);
		}
	}

	void showCar(Mat &img){
		if(car){
			vector<Scalar> vcolor;
			vcolor.push_back(Scalar(0, 255, 0));
			vcolor.push_back(Scalar(0, 0, 255));
			drawCar(*car, img, vcolor);
		}
	}

	~Data(){
		clear();
	}

	void clear(){
		//--------------------free pointer-------------------------
		clearLaneData();
		clearCarData();
	}

	void clearLaneData(){
		for (int i = 0; i < interpolatedLines.size(); ++i,interpolatedLines.pop_back())
			delete interpolatedLines.back();
		interpolatedLines.clear();

		for (int i = 0; i < lanes.size(); ++i, lanes.pop_back())
			delete lanes.back();
		lanes.clear();
	}

	void clearCarData(){
		if(car){
			delete car;
			car = NULL;
		}
	}
};



#endif /* DATASTRUCTURE_H_ */
