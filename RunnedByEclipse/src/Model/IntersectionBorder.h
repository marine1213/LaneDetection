/*
 * IntersectionBorder.h
 *
 *  Created on: Nov 8, 2016
 *      Author: gnouchnam
 */

#ifndef INTERSECTIONBORDER_H_
#define INTERSECTIONBORDER_H_

#include <opencv2/imgproc.hpp>
#include "LinesInterpolator.h"
#include "LinesConnector.h"

/**
 * extended borders limit value
 * Inverse lines at opposite lane area
 * add timer to make the car stop at first stop line
 */
class IntersectionBorder{
// 4 lines bound the border
// 2 set of interpolated lines which is equivalent to 2 lanes of 1 road direction
private:
	Line *borderLine;

	Lane *laneData[4];
	int borderPosId;

	LinesInterpolator *ipl[2];

	// bounding values
	float limLeft, limTop, limRight, limBottom;

	void setLaneData(int id, Lane *newLane, Rect *rect = NULL){

		if(!laneData[id]){
			int left 	= rect->x;
			int top		= rect->y;
			int right 	= rect->x + rect->width;
			int bottom 	= rect->y + rect->height;

			float limLeftTemp, limTopTemp, limRightTemp, limBottomTemp;

			limLeftTemp		= limLeft 	= (limLeft<0)? left:(limLeft > left)? left : limLeft;
			limRightTemp	= limRight 	= (limRight<0)? right:(limRight < right)? right : limRight;
			limTopTemp		= limTop 	= (limTop<0)? top:(limTop > top)? top : limTop;
			limBottomTemp	= limBottom = (limBottom<0)? bottom:(limBottom < bottom)?bottom: limBottom;


			if(rect){
				switch (borderPosId) {
					case BORDER_LEFT:
						limLeftTemp		= limLeft  	= left 	 = 0;
						limBottomTemp	= borderLine->getPoint2()[1];
						limTopTemp		= 0;
						break;
					case BORDER_TOP:
						limTopTemp		= limTop   	= top 	 = 0;
						limRightTemp	= borderLine->getPoint2()[1];
						limLeftTemp		= 0;
						break;
					case BORDER_RIGHT:
						limRightTemp	= limRight 	= right	 = borderLine->getPoint2()[0];
						limBottomTemp	= borderLine->getPoint2()[1];
						limTopTemp		= 0;
						break;
					case BORDER_BOTTOM:
						limBottomTemp	= limBottom	= bottom = borderLine->getPoint2()[1];
						limRightTemp	= borderLine->getPoint2()[0];
						limLeftTemp		= 0;
						break;
					default:
						assert("Wrong border ID" == NULL);
						break;
				}
			}
			float exception[4] = {limLeftTemp, limTopTemp, limRightTemp, limBottomTemp};
			newLane->setLimitedArea(left,top,right,bottom, NULL, exception);
			laneData[id] = newLane;
		}
		else
			cout<<"duplicate data at lane no "<<id<<endl;
	}

public:

	IntersectionBorder(Line *border, int borderId){
		borderLine 	= border;
		borderPosId	= borderId;
		for (int i = 0; i < 4; ++i)
			laneData[i] = NULL;
		for (int i = 0; i < 2; ++i) {
			ipl[i] = NULL;
		}
		limLeft = limRight = limTop = limBottom = -1;
	}

	~IntersectionBorder(){

		cout<<"Intersection border Destructor"<<endl;
		if(borderLine)
			delete borderLine;
		for (int i = 0; i < 4; ++i)
			if(laneData[i])
				delete laneData[i];
		for (int i = 0; i < 2; ++i)
			if(ipl[i])
				delete ipl[i];
		cout<<"stopLane deleted"<<endl;
	}

	//===================== MVO detection support part =============================

	/**
	 * Return id of the lane that the point belongs to.
	 * From Left to Right, lane ids are: 0, 1 and so on (for more lanes detected)
	 * If the point does not belong to this border area, this function return -1.
	 */
	int within(float x, float y, bool extendedBorder = false){
		float rx = roundf(x);
		float ry = roundf(y);

		// extend cover area at input lane
		float tempLimLeft = limLeft;
		float tempLimTop = limTop;
		float tempLimRight = limRight;
		float tempLimBottom = limBottom;

		switch (borderPosId) {
			case BORDER_BOTTOM:
				tempLimBottom = limBottom*1.5;
				break;
			case BORDER_TOP:
				tempLimTop = -limBottom;
				break;
			case BORDER_LEFT:
				tempLimLeft = -limRight;
				break;
			case BORDER_RIGHT:
				tempLimRight = limRight*1.5;
				break;
			default:
				assert("wrong border id at within border"!=0);
				break;
		}
		if(rx >= tempLimLeft && rx <= tempLimRight && ry >= tempLimTop && ry <= tempLimBottom){
			float d1 = laneData[0]->getCenter()->distance(rx,ry,true);
			for (int i = 1; i < 3; ++i) {
				float d2 = laneData[i]->getCenter()->distance(rx,ry,true);
				if(d1*d2 <= 0)	// the point stay inside area of 2 lines
					return (i-1);
				d1 = d2;
			}
		}
		return -1;
	}

	int getCenterIntersectionAreaEdge(){
		switch (borderPosId) {
			case BORDER_LEFT:
				return limRight;
			case BORDER_TOP:
				return limBottom;
			case BORDER_RIGHT:
				return limLeft;
			case BORDER_BOTTOM:
				return limTop;
			default:
				assert("Wrong id border - intersection edge"!=0);
				return -1;
		}
	}

	//============================ Direction part ==================================
	/**
	 * Create connector between this border area to destination border area
	 */
	LinesConnector* pathTo(IntersectionBorder *destination){
		float *ipts = destination->getInputPts();
		float *opts = getOutputPts();
		return new LinesConnector(opts, ipts);
	}

	float* getInputPts(){
		return ipl[IPLS_INPUT]->getPtsInIntersection();
	}

	float* getOutputPts(){
		return ipl[IPLS_OUTPUT]->getPtsInIntersection();
	}

	//================================ Setup part ==================================
	void interpolateLines(Mat *img){
		int extendLeft, extendRight, extendTop, extendBot;
		int eVal = 30;

		for (int i = 0; i < 2; ++i) {
			extendBot = extendLeft = extendTop = extendRight = 0;
			Line *cL1 = laneData[i]->getCenter();
			Line *cL2 = laneData[i+1]->getCenter();
			// extend ipl for turning
			if(i == IPLS_INPUT)
				switch (borderPosId) {
					case BORDER_LEFT:
						extendRight = eVal;
						break;
					case BORDER_TOP:
						extendBot	= eVal;
						break;
					case BORDER_RIGHT:
						extendLeft	= eVal;
						break;
					case BORDER_BOTTOM:
						extendTop	= eVal;
						break;
					default:
						assert("Wrong id border - interpolate lines"!=0);
				}
			ipl[i]	= new LinesInterpolator(new Line(*cL1), new Line(*cL2),borderPosId);
			ipl[i]->setLimitedArea(limLeft - extendLeft, limTop - extendTop, limRight + extendRight, limBottom + extendBot, img);
			// this is going out lane, rearrange the order of left and right lines
			if(i == IPLS_INPUT)
				ipl[i]->inverseOrder();
		}
	}

	bool isLaneDataValid(){
		for (int i = 0; i < 4; ++i) {
			if(!laneData[i])
				return false;
		}
		return true;
	}

	bool isInterpolatedDataValid(){
		for (int i = 0; i < 2; ++i) {
			if(!ipl[i] || !ipl[i]->isValid())
				return false;
		}
		return true;
	}

	LinesInterpolator** getIpl() {
		return ipl;
	}

	Line* getBorderLine() {
		return borderLine;
	}


	Lane* getLeftLane() {
		return laneData[LANE_LEFT];
	}

	void setLeftLane(Lane* leftLane, Rect *rect = NULL) {
		setLaneData(LANE_LEFT,leftLane, rect);
	}

	Lane* getRightLane() {
		return laneData[LANE_RIGHT];
	}

	void setRightLane(Lane* rightLane, Rect *rect = NULL) {
		setLaneData(LANE_RIGHT, rightLane, rect);
	}

	Lane* getStopLane() {
		return laneData[LANE_STOP];
	}

	void setStopLane(Lane* stopLane, Rect *rect = NULL) {
		setLaneData(LANE_STOP, stopLane, rect);
	}

	Lane* getYellowLane() {
		return laneData[YELLOW_LANE];
	}

	void setYellowLane(Lane* yellowLane, Rect *rect = NULL) {
		setLaneData(YELLOW_LANE, yellowLane, rect);
	}
};


#endif /* INTERSECTIONBORDER_H_ */
