/*
 * Lane.h
 *
 *  Created on: Oct 14, 2016
 *      Author: gnouchnam
 */

#ifndef LANE_H_
#define LANE_H_

#include "Line.h"

enum LaneType {
	WHITE_LANE = 0, YELLOW_LANE = 1
};

class Lane {
private:

	Line *leftLine, *rightLine, *centerLine;
	LaneType type;
	float distanceToO, angleToO;

public:

	Lane(Line line1, Line line2, LaneType newType) {
		leftLine = (Line*) malloc(sizeof(Line));
		rightLine = (Line*) malloc(sizeof(Line));
		centerLine = (Line*) malloc(sizeof(Line));

		float a1 = line1.getAngle();
		float a2 = line2.getAngle();
		if (a1 > a2 ) {
			memcpy(leftLine, &line1, sizeof(line1));
			memcpy(rightLine, &line2, sizeof(line2));
		} else {
			memcpy(rightLine, &line1, sizeof(line1));
			memcpy(leftLine, &line2, sizeof(line2));
		}
		type = newType;
		distanceToO = distance(0,0);
		angleToO = (a1 + a2)/2;
		centerLine = leftLine->findMidLine(*rightLine);
	}

	~Lane() {
		free(leftLine);
		free(rightLine);
		free(centerLine);
	}

	float distance(int x,int y){
		float dl = leftLine->distance(x,y);
		float dr = rightLine->distance(x,y);
		return (dl+dr)/2;
	}

	bool isOn(Lane &lane){
		bool l1 = lane.leftLine->isOn(*leftLine);
		bool l2 = lane.leftLine->isOn(*rightLine);
		bool l3 = lane.rightLine->isOn(*rightLine);
		bool l4 = lane.rightLine->isOn(*leftLine);
		return (l1||l2||l3||l4);
	}

	Line* getLeft(){ return leftLine;}
	Line* getRight(){ return rightLine;}
	Line* getCenter(){ return centerLine;}

	LaneType getType(){ return type;}
	float getDistanceToO(){ return distanceToO;}
	float getAngle(){ return angleToO;}
};

#endif /* LANE_H_ */
