
/*
 * Car.h
 *
 *  Created on: Oct 14, 2016
 *      Author: gnouchnam
 */

#ifndef CAR_H_
#define CAR_H_


#include "Intersection.h"
#include "../Controller/CarCtrl.h"
#include "Path.h"
#include "PointSeries.h"

class Car{
public:
	enum SideStateEvent{
		LEFT_LANE_CRASH,
		LEFT_LINE_WARNING,
		RIGHT_LINE_WARNING,
		RIGHT_LANE_CRASH
	};

	enum FrontStateEvent{
		NEAR_STOP_LINE,
		ON_STOP_LINE
	};

private:
	float tires[8];
	float centerPt[2], diffCenter[2];
//	float lanesDistance[4], diffLanesDist[4];
//	float linesDistance[4], diffLinesDist[4];
	PointSeries *buff2Pts, *buff500Pts;

//	float tireAngles[2];
	float relativeAngle;

	int lfId,lrId,rfId,rrId;
//	float driveAngle, speed;

	float balanceDistance;
	void (*sideState_cb)(SideStateEvent event,  Car *dispatcher, CarCtrl &ctrl);
	void (*frontState_cb)(FrontStateEvent event, Car *dispatcher, CarCtrl &ctrl);

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
		diffCenter[0] = abs(cx - centerPt[0]);
		diffCenter[1] = abs(cy - centerPt[1]);
		centerPt[0] = cx;
		centerPt[1] = cy;
		buff500Pts->add(cx,cy);
		buff2Pts->add(cx,cy);
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
	Path path;
	bool stopAtFirstStopLine;

	int laneId;
	LinesInterface	*myIpl;

	float getMinDistanceOf4pts(Line *stopLine){
		float minDistance = stopLine->distance(tires[0],tires[1]);
//		float sig1 = minDistance<0?-1:1, sig2 = 1;
		for (int i = 2; i < 8; i+=2) {
			float distance = stopLine->distance(tires[i],tires[i+1]);
//			if((i/2)&1)
//				sig2 *= distance<0?-1:1;
//			else
//				sig1 *= distance<0?-1:1;
			if(minDistance > distance)
				minDistance = distance;
		}
//		minDistance *= ((sig1 == -1 || sig2 == -1)? -1 : 1);
		return minDistance;
	}

	//================== locating functions ========================
	void findMyIntersectionArea(Intersection *intersection, CarCtrl &carctl){
			path.setIntersection(intersection);
			int borderId = laneId = -1;
			for (int i = 0; i < 4; ++i) {
				int withinId = intersection->getBorders()[i]->within(centerPt[0],centerPt[1],true);
				if(withinId == -1)
					continue;
				else{
					borderId = i;
					laneId	= withinId;
					// when the car knows where it is and where it should go
					// setup connection between 2 areas
					break;
				}
			}
			if(intersection->withinCeterArea(centerPt[0],centerPt[1]))
				borderId = INTERSECTION;
			path.setCurrentBorder(borderId, carctl);
		}

	bool usingIpl;
		void findMyInterpolatedLines(Intersection *intersection) {
			if(path.getCurrentBorderId() == INTERSECTION){
				cout<<"chane ipl to connector"<<endl;
				int destBorderId = path.getDestinationBorderId();
				if(path.isBorderIdValid(destBorderId))
					myIpl = intersection->getBorders()[destBorderId]->getIpl()[IPLS_INPUT];
//				myIpl = path.getLinesConnector();
				usingIpl = false;
			}
			else if(isPositionValid()){	// INTERSECTION will throw null pointer when get border
				//cout<<"using ipl"<<endl;
				myIpl = intersection->getBorders()[path.getCurrentBorderId()]->getIpl()[laneId];
				usingIpl = true;
			}
		}
		//=======================================================

public:


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
		balanceDistance = -1;

		laneId 		= -1;
		myIpl 		= 0;
		stopAtFirstStopLine = true;
		usingIpl	= true;
		buff500Pts 	= new PointSeries(500);
		buff2Pts	= new PointSeries(2);

		updateCoor(x1,y1,x2,y2,x3,y3,x4,y4,cx,cy);
	}

	~Car(){
		delete buff500Pts;
		delete buff2Pts;
//		if(leftLine)
//			delete leftLine;
//		if(rightLine)
//			delete rightLine;
	}

	void updateCoor(float	x1	,float	y1
			,float	x2	,float	y2
			,float	x3	,float	y3
			,float	x4	,float	y4
			,float  cx  ,float  cy){
		// to detect if the car is moving or not
		setCenter(cx,cy);
		setCoor(x1,y1,x2,y2,x3,y3,x4,y4);
		arrangeId();
//		shrink();
//		if(leftLine)
//			delete leftLine;
//		if(rightLine)
//			delete rightLine;
//		leftLine = new Line(tires[lfId],tires[lfId+1],tires[lrId],tires[lrId+1]);
//		rightLine = new Line(tires[rfId],tires[rfId+1],tires[rrId],tires[rrId+1]);
	}

	void setDestinationBorder(int destinationBorderId){
		path.setDestinationBorder(destinationBorderId);
	}

	/**
	 * To make the car stop before entering intersection
	 */
	void distanceToStopLines(Intersection *intersection, CarCtrl &carCtl){
		int currentBorderId = path.getCurrentBorderId();
		if(path.isBorderIdValid(currentBorderId)){
			Lane *stopLane = intersection->getBorders()[path.getCurrentBorderId()]->getStopLane();
			float dStop = getMinDistanceOf4pts(stopLane->getCenter());
#if CHECK_CENTER_PT
			cout<<"centerPt: "<<centerPt[0]<<" - "<<centerPt[1]<<endl;
#endif
			carCtl.dStop = relativeAngle;
			if(relativeAngle < 5){
				path.setBigTurn(false);
			}
#if CHECK_DISTANCE2STOPLINE
			if(abs(dStop) >0)
				cout<<"To stop line = "<<dStop<<endl;
#endif
			//TODO-bug triggered by other objects
			//if(carCtl.isWorking){
			if (dStop < 50 && dStop > 0 && path.getCurrentBorderId() == path.getStartBorderId()){
				if (frontState_cb)
					frontState_cb(ON_STOP_LINE, this, carCtl);
			}else
			if (dStop < 180 && dStop > 0 && path.getCurrentBorderId() == path.getStartBorderId())
				if (frontState_cb)
					frontState_cb(NEAR_STOP_LINE, this, carCtl);
//			}
		}
		if(currentBorderId == INTERSECTION){
			Lane *stopLane = intersection->getBorders()[path.getDestinationBorderId()]->getStopLane();
			float dStop = getMinDistanceOf4pts(stopLane->getCenter());
//			cout<<"angleInt:"<<relativeAngle<<endl;
			carCtl.dStop = relativeAngle;// dStop;
		}
	}

	void angleToDestinationLane(Intersection *intersection){
		if(buff2Pts->size() == 2){
			Line *cLine = intersection->getBorders()[path.getDestinationBorderId()]->getIpl()[IPLS_INPUT]->getCenterExLine();
			Line tempLine = Line(buff2Pts->get(1)[0],buff2Pts->get(1)[1],buff2Pts->get(0)[0],buff2Pts->get(0)[1]);
			relativeAngle = tempLine.angle(*cLine);
			cout<<"angle:"<<relativeAngle<<endl;
		}
	}

	/**
	 * turn the car left or right in the middle of intersection
	 */
	void turnTheCar(Intersection *intersection, CarCtrl &carCtl){
		angleToDestinationLane(intersection);
		path.turnTheCar(carCtl);
	}

	/**
	 * To keep the car going stable on the road
	 */
	void distanceToInterpolatedLines(LinesInterface *ipl, CarCtrl &carCtl){
		if(!ipl)		//validate ipl
			return;
		// calculate distance
		balanceDistance = ipl->getCenterExLine()->distance(centerPt[0],centerPt[1]);
		float ctDistLeft = ipl->getLeftExLine()->distance(centerPt[0],centerPt[1],true);
		float ctDistRight = ipl->getRightExLine()->distance(centerPt[0],centerPt[1],true);

		// turning direction selection
		int leftOrRight = abs(ctDistRight) > abs(ctDistLeft) ? -1 : 1;
		int insideOrOut = signbit(ctDistRight * ctDistLeft);
#if CHECK_INSIDE_OR_OUTSIDE_OF_IPL
		cout << "inside = "<< insideOrOut<<endl;
#endif
		if (balanceDistance > 1) {
			if (leftOrRight == -1 && ctDistLeft <= 5) {
				// turn right
				if(sideState_cb)
					sideState_cb(LEFT_LINE_WARNING, this, carCtl);
				cout << "Warning left lane: Dleft="<<ctDistLeft<<" Dright="<<ctDistRight<< endl;
			}
			if (leftOrRight == 1 && ctDistRight <= 5) {
				// turn left
				if(sideState_cb)
					sideState_cb(RIGHT_LINE_WARNING, this, carCtl);
				cout << "Warning right lane: Dleft="<<ctDistLeft<<" Dright="<<ctDistRight<< endl;
			}
			// not consider outer area yet
		} //else
	}


	//==================== locate nearest border to the car =====================
	/**
	 * Locate current border position of car and
	 * find appropriate interpolated lines
	 */
	void findMyPosition(Intersection *intersection, CarCtrl &carCtl){
		findMyIntersectionArea(intersection, carCtl);
		findMyInterpolatedLines(intersection);
	}

	bool isPositionValid(){
		return (path.getCurrentBorderId()!= -1 && laneId != -1);
	}


	bool isMoving(){
		return sqrt((pow(diffCenter[0],2) + pow(diffCenter[1],2))) > 1;
	}

	bool isUsingIpl(){
		return usingIpl;
	}
	//=========================== getters area =======================
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

	int getLaneId() const {
		return laneId;
	}

	Path& getPath()  {
		return path;
	}

	LinesInterface* getIplines() const {
		return myIpl;
	}

	void setFrontStateCb(void (*func_cb)(FrontStateEvent event,Car *dispatcher, CarCtrl &ctrl)){
		frontState_cb = func_cb;
	}

	void setSideStateCb(void (*func_cb)(SideStateEvent event,Car *dispatcher, CarCtrl &ctrl)){
		sideState_cb = func_cb;
	}

	bool isStopAtFirstStopLine() const {
		return stopAtFirstStopLine;
	}

	void setStopAtFirstStopLine(bool stopAtFirstStopLine) {
		this->stopAtFirstStopLine = stopAtFirstStopLine;
	}

	PointSeries* getBuff100Pts()  {
		return buff500Pts;
	}

	PointSeries* getBuff2Pts()  {
		return buff2Pts;
	}
};


#endif /* CAR_H_ */
