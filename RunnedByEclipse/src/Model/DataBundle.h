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
#include "../Controller/CarComm.h"
#include "CarElements/Car.h"
#include "IntersectionElements/Line.h"
#include "IntersectionElements/Lane.h"
#include "IntersectionElements/Intersection.h"



class DataBundle{
private:
	Car *carData;

	Intersection *intersection;

	Ptr< BackgroundSubtractor> pMOG2;

	Mat tmpDetectionImg;

	Mat transformMat;
	Mat input;
public:


	Mat* getTmpDetectionImg(){
		return &tmpDetectionImg;
	}
	Mat& getInput(){
		return input;
	}

	Mat& getTransformMat(){
		return transformMat;
	}

	void setupIntersection(int imgWidth, int imHeight){
		intersection = new Intersection(imgWidth, imHeight);
		tmpDetectionImg = Mat::zeros(Size(imgWidth, imHeight), CV_8UC3);
	}

	Intersection* getIntersection(){
		return intersection;
	}

	Ptr< BackgroundSubtractor>& getPMog2(){
		return pMOG2;
	}

	DataBundle(){
		pMOG2 	= createBackgroundSubtractorMOG2(500,16,false);
		carData 	= NULL;
		intersection = NULL;
	}

	void createCarObject(Car *newCarObj){
		carData = newCarObj;
	}

	Car* getCarData(){
		return carData;
	}

	void showIntersection(Mat &img){
		for (int i = 0; i < 4; ++i) {
			if(intersection->getBorders()[i]->isLaneDataValid())
				drawIntersectionBorder(*intersection->getBorders()[i],img);
			else{
#if SHOW_PROCESSING_STATE
				cout<<"invalid border "<<i<<endl;
#endif
			}
		}
//		cout<<"draw intersection completed"<<endl;
	}

	void showInterpolatedLineData(Mat &img){
		for (int i = 0; i < 4; ++i) {
			if(intersection->getBorders()[i]->isInterpolatedDataValid())
				drawInterpolatedBorderLines(*intersection->getBorders()[i],img);
			else{
#if SHOW_PROCESSING_STATE
				cout<<"invalid lines in border "<<i<<endl;
#endif
			}
		}
	}

	void showCar(Mat &img){
		if(carData){
			vector<Scalar> vcolor;
			vcolor.push_back(Scalar(0, 255, 0));
			vcolor.push_back(Scalar(0, 0, 255));
			drawCar(*carData, img, vcolor);
		}
	}

	~DataBundle(){
#if SHOW_CLASSES_STATE
		cout<<"Data Destructor"<<endl;
#endif
		clear();
		clearIntersectionData();
	}

	void clear(){
		//--------------------free pointer-------------------------
		clearCarData();
	}

	void clearCarData(){
		if(carData){
			delete carData;
			carData = NULL;
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
