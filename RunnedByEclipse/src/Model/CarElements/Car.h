
/*
 * Car.h
 *
 *  Created on: Oct 14, 2016
 *      Author: gnouchnam
 */

#ifndef CAR_H_
#define CAR_H_


#include "PointSeries.h"
#include "../../ExternalAlgorithm/KalmanAdapter.h"

class Car{
public:


private:
	vector<Point2f> tires;
	float centerPt[2], diffCenter[2];
	float wheelBase;

	PointSeries *buff2Pts, *buff50Pts;
	PointSeries *buff2Kal, *buff50Kal;
	PointSeries *buff50Diff;

	float relativeAngle;

	bool arrivedDestination;
	bool initialized;
	bool keepWheelBaseValue;

	KalmanAdapter kAdapt;

		//=======================================================

public:

	Car(){
		buff50Pts 	= new PointSeries(50);
		buff2Pts	= new PointSeries(2);
		buff50Kal	= new PointSeries(50);
		buff2Kal	= new PointSeries(2);
		buff50Diff	= new PointSeries(50);
		arrivedDestination = false;
		initialized	= false;
		keepWheelBaseValue = false;

		relativeAngle 	= 0;
		wheelBase		= -1;
//		updateCoor(cx,cy);
	}

	~Car(){
		delete buff50Pts;
		delete buff2Pts;
		delete buff2Kal;
		delete buff50Kal;
	}

	void updateCoor(vector<Point2f> mpt){
		tires = mpt;
	}

	void setCenter(float cx  ,float cy, float time){
		if(!initialized){
			// to detect if the car is moving or not
			centerPt[0] = cx;
			centerPt[1] = cy;
			initialized = true;
		}
		diffCenter[0] = abs(cx - centerPt[0]);
		diffCenter[1] = abs(cy - centerPt[1]);
		buff50Diff->add(diffCenter[0],diffCenter[1],time);

		centerPt[0] = cx;
		centerPt[1] = cy;
		buff50Pts->add(cx,cy, time);
		buff2Pts->add(cx,cy, time);

		Point2f kpt = kAdapt.update(cx,cy);
		buff2Kal->add(kpt.x, kpt.y, time);
		buff50Kal->add(kpt.x, kpt.y, time);
	}

	bool isMoving(){
		float movDistance = sqrt((pow(diffCenter[0],2) + pow(diffCenter[1],2)));
		return  (movDistance > 1);
	}

	void clearBuffer(){
		buff2Kal->clear();
		buff2Pts->clear();
		buff50Diff->clear();
		buff50Kal->clear();
		buff50Pts->clear();
	}
	//=========================== getters area =======================
	vector<Point2f> getTires() const{
		return tires;
	}
	const float* getCenterPt() const {
		return centerPt;
	}

	PointSeries* getBuff50Kal(){
		return buff50Kal;
	}

	PointSeries* getBuff50Pts()  {
		return buff50Pts;
	}

	PointSeries* getBuff2Kal(){
		return buff2Kal;
	}

	PointSeries* getBuff50Diff(){
		return buff50Diff;
	}

	bool hasArrivedDestination(){
		return arrivedDestination;
	}

	void setArrivedDestination(bool arrived){
		arrivedDestination = arrived;
	}

	float getRelativeAngle(){
		return relativeAngle;
	}

	void setRelativeAngle(float angle){
		relativeAngle = angle;
	}

	void setWheelBase(float value){
		if(!keepWheelBaseValue)
			wheelBase = value;
	}

	float getWheelBase(){
		return wheelBase;
	}

	void keepWheelBase(){
		keepWheelBaseValue = true;
	}
};


#endif /* CAR_H_ */
