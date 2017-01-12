/*
 * CarCtrl.h
 *
 *  Created on: Dec 29, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_CONTROLLER_CARCTRL_H_
#define SRC_CONTROLLER_CARCTRL_H_


#include "../Model/CarElements/Path.h"
#include "../Model/DataBundle.h"
#include "../Threads/DriveThread.h"

class CarCtrl{

private:
	DataBundle *data;

	Path path;
	CarComm carComm;
	Path::TurningControl direction;

	LinesInterface	*myIpl;
	DriveThread		*driveThread;


public:

	CarCtrl(DataBundle *sourceData){
		myIpl 				= 0;
		data 				= sourceData;
		direction			= Path::GOING_STRAIGHT;				// going straight
		driveThread			= new DriveThread(&carComm);
	}

	~CarCtrl(){
		delete driveThread;
	}

	//==================== locate nearest border to the car =====================
	/**
	 * Locate current border position of car and find appropriate interpolated lines
	 */
	void findMyPosition(){
		//------------------------------------------------------------
		// find which area that the car belong to
		//------------------------------------------------------------
		int borderId = -1;
		for (int i = 0; i < 4; ++i) {
			int withinId = data->getIntersection()->getBorders()[i]->within(data->getCarData()->getCenterPt()[0],data->getCarData()->getCenterPt()[1],true);
			if(withinId == -1)
				continue;
			else{
				borderId = i;
				path.setLaneId(withinId);
				// when the car knows where it is and where it should go
				// setup connection between 2 areas
				break;
			}
		}
		if(data->getIntersection()->withinCeterArea(data->getCarData()->getCenterPt()[0],data->getCarData()->getCenterPt()[1]))
			borderId = INTERSECTION;
		path.setCurrentBorder(borderId);
		//------------------------------------------------------------
		// find appropriate interpolated lines for the car
		//------------------------------------------------------------
		if (path.getCurrentBorderId() == INTERSECTION) {
			int destBorderId = path.getDestinationBorderId();
			if (path.isBorderIdValid(destBorderId))
				myIpl = data->getIntersection()->getBorders()[destBorderId]->getIpl()[IPLS_INPUT];
		} else if (path.isPositionValid()) {// INTERSECTION will throw null pointer when get border
			myIpl = data->getIntersection()->getBorders()[path.getCurrentBorderId()]->getIpl()[path.getLaneId()];
		}
	}


	//================== find the line for turning ========================

	Line* getTurningLine(){
		// if car intention is not to turn, return
		// if distance to turning line < threshold = 20, set big turning
		// if relative angle with destination center < threshold = 3, keep balance

		// first
//		if(!path.isPathValid())
//			return NULL;

		direction = path.getDirectionArg();
		if(direction != Path::TURNING_LEFT && direction != Path::TURNING_RIGHT)
			return NULL;

		// second
		IntersectionBorder *ib = data->getIntersection()->getBorders()[path.getDestinationBorderId()];

		// get trigger turning line
		if(direction == Path::TURNING_LEFT){
			return ib->getYellowLane()->getCenter();
		}
		if(direction == Path::TURNING_RIGHT){
			return ib->getLeftLane()->getCenter();
		}
		return NULL;
	}
	//=========================================================================

	//=================== find angle for calibrating ==========================
	void angleToNextCenterLine(){
		LinesInterface *lineIntf = NULL;
		if (data->getCarData()->getBuff2Kal()->size() == 2 && data->getCarData()->isMoving()) {
			lineIntf = data->getIntersection()->getBorders()[path.getDestinationBorderId()]->getIpl()[IPLS_INPUT];
			if (lineIntf) {
				Line *cLine = lineIntf->getCenterExLine();
				float *rearPt = data->getCarData()->getBuff2Kal()->get(1);
				float *frontPt = data->getCarData()->getBuff2Kal()->get(0);
				Line tempLine = Line(rearPt[0], rearPt[1], frontPt[0], frontPt[1]);
				data->getCarData()->setRelativeAngle(tempLine.angle(*cLine));
#if SHOW_PROCESSING_STATE
				cout << "angle:" << relativeAngle << endl;
#endif
			}
		}
	}
	//=========================================================================
	DriveThread* getDriveThread(){
		return driveThread;
	}
	/**
	 * result stored after turning line
	 */
	Path::TurningControl getDirection(){
		return direction;
	}

	Path& getPath() {
		return path;
	}

	LinesInterface* getIplines() const {
		return myIpl;
	}

	CarComm& getCarComm() {
		return carComm;
	}

	DataBundle* getData(){
		return data;
	}
};


#endif /* SRC_CONTROLLER_CARCTRL_H_ */
