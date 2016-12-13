/*
 * DataStructure.h
 *
 *  Created on: Oct 25, 2016
 *      Author: gnouchnam
 */

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#include "../common.h"
#include "../DrawHelper/draw_helper.h"
#include "Line.h"
#include "Lane.h"
#include "Car.h"
#include "Intersection.h"
#include "../Controller/ControlInstructor.h"

class Data{
private:
	Car *car;
public:
	Ptr< BackgroundSubtractor> pMOG2;

	CarCtrl *carCtl;

	Intersection *intersection;
	Mat tmpDetectionImg, tmpMvoImg;
	Mat transformMat;

	Data(){
		pMOG2 	= createBackgroundSubtractorMOG2(500,16,false);
		car 	= NULL;
		carCtl 	= new CarCtrl();
		intersection = NULL;
	}

	void createCarObject(Car *newCarObj){
		car = newCarObj;
		car->setFrontStateCb(frontStateResponse);
		car->setSideStateCb(sideStateResponse);
		car->getPath().setAreaChangeCb(areaChangeResponse);
	}
	void setupIntersection(int imgWidth, int imHeight){
		intersection = new Intersection(imgWidth, imHeight);
	}

	void overLaneAssessment(int destinationBorderId){
		if(car){
#if CHECK_LANE_LOCATOR
			cout<<"my borderId:"<<car->getPath().getCurrentBorderId()<<" my lane Id:"<<car->getLaneId()<<endl;
#endif
			car->findMyPosition(intersection, *carCtl);
			if(!car->isPositionValid()){
				cout<<"Locating  nearest intersection border"<<endl;
//				cout<<"assessment pos: border="<<car->getBorderId()<<" laneId="<<car->getLaneId()<<endl;
			}
			car->setDestinationBorder(destinationBorderId);
			if(car->getPath().isBigTurn())
				car->turnTheCar(intersection, *carCtl);
			else
				car->distanceToInterpolatedLines(car->getIplines(), *carCtl);
			car->distanceToStopLines(intersection,*carCtl);
		}
	}

	void showIntersection(Mat &img){
		for (int i = 0; i < 4; ++i) {
			if(intersection->getBorders()[i]->isLaneDataValid())
				drawIntersectionBorder(*intersection->getBorders()[i],img);
			else
				cout<<"invalid border "<<i<<endl;
		}
//		cout<<"draw intersection completed"<<endl;
	}

	void showInterpolatedLineData(Mat &img){
		for (int i = 0; i < 4; ++i) {
			if(intersection->getBorders()[i]->isInterpolatedDataValid())
				drawInterpolatedBorderLines(*intersection->getBorders()[i],img);
			else
				cout<<"invalid lines in border "<<i<<endl;
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

	Car* getCarObj(){
		return car;
	}

	~Data(){
		cout<<"Data Destructor"<<endl;
		clear();
		clearIntersectionData();
	}

	void clear(){
		//--------------------free pointer-------------------------
		clearCarData();
	}

	void clearCarData(){
		if(car){
			delete car;
			car = NULL;
		}
	}

	void clearIntersectionData(){
		if(intersection){
			delete intersection;
			intersection = NULL;
		}
	}
};



#endif /* DATASTRUCTURE_H_ */
