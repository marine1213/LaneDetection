
/*
 * Car.h
 *
 *  Created on: Oct 14, 2016
 *      Author: gnouchnam
 */

#ifndef CAR_H_
#define CAR_H_


#include "Line.h"
#include "Lane.h"
#include "../Controller/CarCtrl.h"

class Car{
private:
	float tires[8];
	float centerPt[2];
	float lanesDistance[4], diffLanesDist[4];
	float linesDistance[4], diffLinesDist[4];

	float tireAngles[2];
	float relativeAngle[4];

	int lfId,lrId,rfId,rrId;
	float driveAngle, speed;

	float distToCenter;

	void setCoor(float	x1	,float	y1
				,float	x2	,float	y2
				,float	x3	,float	y3
				,float	x4	,float	y4){
		tires[lfId] 	= x1;
		tires[lfId+1] 	= y1;
		tires[rfId] 	= x2;
		tires[rfId+1] 	= y2;
		tires[rrId] 	= x3;
		tires[rrId+1] 	= y3;
		tires[lrId] 	= x4;
		tires[lrId+1] 	= y4;
	}

	void setCenter(float cx  ,float cy){
		centerPt[0] = cx;
		centerPt[1] = cy;
	}

	void arrangeId(){
		for (int i = 0; i < 7; i+=2) {
			if(tires[i]<centerPt[0] && tires[i+1]<centerPt[1])
				lfId = i;
			else if(tires[i]>centerPt[0] && tires[i+1]<centerPt[1])
				rfId = i;
			else if(tires[i]<centerPt[0] && tires[i+1]>centerPt[1])
				lrId = i;
			else if(tires[i]>=centerPt[0] && tires[i+1]>=centerPt[1])
				rrId = i;
		}
	}

	void shrink(){
		tires[lfId] 	*= 1.05;
		tires[lfId+1] 	*= 1.05;
		tires[rfId] 	*= 0.98;
		tires[rfId+1] 	*= 1.05;
		tires[rrId] 	*= 0.98;
		tires[rrId+1] 	*= 0.98;
		tires[lrId] 	*= 1.02;
		tires[lrId+1] 	*= 0.95;
	}
public:

	int leftLaneId, rightLaneId;
	int centerLineId;
//	Line *leftLine, *rightLine;

	Car(float	x1	,float	y1
		,float	x2	,float	y2
		,float	x3	,float	y3
		,float	x4	,float	y4
		,float  cx  ,float  cy){
		lfId = 0;
		lrId = 2;
		rfId = 4;
		rrId = 6;
//		leftLine = 0;
//		rightLine = 0;
		updateCoor(x1,y1,x2,y2,x3,y3,x4,y4,cx,cy);
		leftLaneId = -1;
		rightLaneId = -1;
		centerLineId = -1;
		distToCenter = -1;
	}
//
//	~Car(){
//		if(leftLine)
//			delete leftLine;
//		if(rightLine)
//			delete rightLine;
//	}

	void updateCoor(float	x1	,float	y1
			,float	x2	,float	y2
			,float	x3	,float	y3
			,float	x4	,float	y4
			,float  cx  ,float  cy){
		setCenter(cx,cy);
		setCoor(x1,y1,x2,y2,x3,y3,x4,y4);
		arrangeId();
		shrink();
//		if(leftLine)
//			delete leftLine;
//		if(rightLine)
//			delete rightLine;
//		leftLine = new Line(tires[lfId],tires[lfId+1],tires[lrId],tires[lrId+1]);
//		rightLine = new Line(tires[rfId],tires[rfId+1],tires[rrId],tires[rrId+1]);
	}

	void distanceToLanes(vector<Lane *> lanes){
		if(lanes.size()){
			float d0 = lanes[leftLaneId]->getLeft()->distance(tires[lfId],tires[lfId+1]);
			float d1 = lanes[leftLaneId]->getLeft()->distance(tires[lrId],tires[lrId+1]);
			float d2 = lanes[rightLaneId]->getLeft()->distance(tires[rfId],tires[rfId+1]);
			float d3 = lanes[rightLaneId]->getLeft()->distance(tires[rrId],tires[rrId+1]);
			diffLanesDist[0] = d0 - lanesDistance[0];
			diffLanesDist[1] = d1 - lanesDistance[1];
			diffLanesDist[2] = d2 - lanesDistance[2];
			diffLanesDist[3] = d3 - lanesDistance[3];
			lanesDistance[0] = d0;
			lanesDistance[1] = d1;
			lanesDistance[2] = d2;
			lanesDistance[3] = d3;
//			cout<<"Lanes:"<<d0<<" - "<<diffLanesDist[0]<<endl;
//			cout<<"Lanes:"<<d1<<" - "<<diffLanesDist[1]<<endl;
//			cout<<"Lanes:"<<d2<<" - "<<diffLanesDist[2]<<endl;
//			cout<<"Lanes:"<<d3<<" - "<<diffLanesDist[3]<<endl;
//			if(diffLanesDist[0] < 0)
//				cout<<"Warning left lane:"<<endl;
//			if(diffLanesDist[2] < 0)
//				cout<<"Warning right lane:"<<endl;
		}
	}

	void distanceToInterpolatedLines(vector<Line*> lines, CarCtrl &carCtl){
		if(lines.size()){
			float ctDistLeft = lines[centerLineId+1]->distance(centerPt[0],centerPt[1]);
			float ctDistRight= lines[centerLineId+2]->distance(centerPt[0],centerPt[1]);
			int leftOrRight = ctDistRight > ctDistLeft?-1:1;
			if (distToCenter > 4) {
				if (leftOrRight == -1 && ctDistLeft <= 3){
					// turn right
					cout << "Warning left lane" << endl;
					carCtl.turnRight();
				}
				if (leftOrRight == 1 && ctDistRight <= 3){
					// turn left
					cout << "Warning right lane" << endl;
					carCtl.turnLeft();
				}
				// not consider outer area yet
			}//else
				// go straight
//				carCtl.goStraight();
//			cout << "Lines:" << ctDistLeft << endl;
//			cout << "Lines:" << ctDistRight << endl;
		}
	}

	void findMyLane(vector<Lane *> lanes){
		if(lanes.size()){
			int k,f;
			// find left lane
			vector<float> dleftList;
			float xleft = t()[getLfId()];
			float yleft = t()[getLfId() + 1];
			for (int i = 0; i < lanes.size(); ++i) {
				float dleft = lanes[i]->getCenter()->distance(xleft, yleft);
				if (dleftList.size() == 0){
					dleftList.push_back(dleft);
					leftLaneId = 0;
				}
				else {
					if (dleft < dleftList[0]) {
						dleftList.pop_back();
						dleftList.push_back(dleft);
						leftLaneId = i;
					}
				}
			}
			// find right lane
			vector<float> dRightList;
			float xRight = t()[getRfId()];
			float yRight = t()[getRfId() + 1];
			for (int i = 0; i < lanes.size(); ++i) {
				float dRight = lanes[i]->getCenter()->distance(xRight, yRight);
				if (dRightList.size() == 0){
					dRightList.push_back(dRight);
					rightLaneId = 0;
				}
				else {
					if (i != leftLaneId && dRight < dRightList[0]) {
						dRightList.pop_back();
						dRightList.push_back(dRight);
						rightLaneId = i;
					}
				}
			}
			cout<< leftLaneId <<" - "<<rightLaneId<<endl;
		}
	}

	void findMyInterpolatedLines(vector<Line *> lines) {
		if (!lines.size())
			return;
		float dct0 = lines[0]->distance(centerPt[0], centerPt[1]);
		centerLineId = 0;
		for (int i = 3; i < lines.size(); i += 3) {
			float dct = lines[i]->distance(centerPt[0], centerPt[1]);
			if (dct < dct0) {
				dct0 = dct;
				centerLineId = i;
			}
		}
		distToCenter = dct0;
	}

	float* t(){
		return tires;
	}

	int getLfId(){
		return lfId;
	}

	int getLrId() const {
		return lrId;
	}

	int getRfId() const {
		return rfId;
	}

	int getRrId() const {
		return rrId;
	}

	const float* getCenterPt() const {
		return centerPt;
	}
};


#endif /* CAR_H_ */
