/*
 * Lane.h
 *
 *  Created on: Oct 14, 2016
 *      Author: gnouchnam
 */

#ifndef LANE_H_
#define LANE_H_

#include "Line.h"

enum LaneColor {
	WHITE_LANE = 4, YELLOW_LANE = 1
};

enum LaneType {
	// interpolated lines and laneData bases on these as ID
	LANE_LEFT = 0, LANE_MID = 1, LANE_RIGHT = 2, LANE_STOP = 3
};

class Lane {
private:

	Line *leftLine, *rightLine, *centerLine;
	LaneColor color;
	LaneType type;
	float distanceToO;

public:

	Lane(Line *line1, Line *line2, LaneColor newColor, bool limitCenterLine = false) {
		float a1 = line1->getAngle();
		float a2 = line2->getAngle();
		if (a1 > a2 ) {
			leftLine = line1;
			rightLine = line2;
		} else {
			rightLine = line1;
			leftLine = line2;
		}
		color = newColor;
		distanceToO = distance(0,0);
		centerLine = leftLine->findMidLine(*rightLine);
	}

	~Lane() {
		cout<<"Lane Destructor"<<endl;
		if(leftLine)
			delete leftLine;
		if(rightLine)
			delete rightLine;
		if(centerLine)
			delete centerLine;
	}

	float distance(int x,int y, bool direction = false){
		float dl = leftLine->distance(x,y,direction);
		float dr = rightLine->distance(x,y,direction);
		return (dl+dr)/2;
	}

	bool isOn(Lane &lane){
		bool l1 = lane.leftLine->isOn(*leftLine);
		bool l2 = lane.leftLine->isOn(*rightLine);
		bool l3 = lane.rightLine->isOn(*rightLine);
		bool l4 = lane.rightLine->isOn(*leftLine);
		return (l1||l2||l3||l4);
	}

	void setLimitedArea(float left, float top,float right, float bottom, Mat *img = NULL, float *exceptionData = NULL){
		leftLine->setLimitedArea(left,top,right,bottom,img,exceptionData);
		rightLine->setLimitedArea(left,top,right,bottom,img,exceptionData);
		centerLine->setLimitedArea(left,top,right,bottom,img,exceptionData);
	}

	Line* getLeft(){ return leftLine;}
	Line* getRight(){ return rightLine;}
	Line* getCenter(){ return centerLine;}

	LaneColor getColor(){ return color;}
	float getDistanceToO(){ return distanceToO;}

	LaneType getType(){	return type;}
	void 	setType(LaneType newType){ type = newType;}
};

#endif /* LANE_H_ */
