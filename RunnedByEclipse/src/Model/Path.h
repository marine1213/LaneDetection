/*
 * Path.h
 *
 *  Created on: Nov 28, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_MODEL_PATH_H_
#define SRC_MODEL_PATH_H_

#include "Intersection.h"
#include "../Controller/CarCtrl.h"



class Path {
public:

	enum TurningControl{
		TURNING_LEFT = 1, GOING_STRAIGHT = 2, TURNING_RIGHT = 3, INVALID_TURNING = -1
	};
	enum PathState{
		INVALID_STATE = -1, DEPARTURE = 1, TRANSIT = 2, ARRIVAL = 3
	};

	enum AreaChangeEvent{
		DEPATURE2TRANSIT,
		TRANSIT2ARRIVAL
	};


private:

	void (*areaChange_cb)(AreaChangeEvent event, Path *dispatcher, CarCtrl &ctrl);

	int startBorderId, destinationBorderId, currentBorderPos;
	TurningControl directionArg;
	PathState carPosState;

	bool bigTurn;

	Intersection	*intersection;
	LinesConnector	*linesConnector;

	//================= setup part ======================
	void createConnector(){
		if(!intersection){
			cout<<"invalid intersection"<<endl;
			return;
		}
		if (linesConnector)
			delete linesConnector;
		if (isPathValid() && startBorderId != destinationBorderId){
			linesConnector = intersection->createPath(startBorderId,destinationBorderId);
			directionArg = (TurningControl)((destinationBorderId - startBorderId + 4) % 4);
		}
		else
			linesConnector = NULL;
	}

	void recognizeCarPosState(int currentBorderId, CarCtrl &carctl){
		if(currentBorderId == startBorderId){
			carPosState = DEPARTURE;
			if(areaChange_cb){
				areaChange_cb(DEPATURE2TRANSIT, this, carctl);
			}
		}else if(currentBorderId == destinationBorderId){
			carPosState = ARRIVAL;
			if(areaChange_cb){
				areaChange_cb(TRANSIT2ARRIVAL, this, carctl);
			}
		}else{
			carPosState = TRANSIT;
		}
	}

	//===========================================

public:


	Path() {
		startBorderId 		= -1;
		destinationBorderId = -1;
		currentBorderPos 	= -1;
		linesConnector 		= NULL;
		intersection		= NULL;
		carPosState			= INVALID_STATE;
		areaChange_cb		= NULL;
		directionArg		= INVALID_TURNING;
		bigTurn				= false;
	}

	~Path(){
		if (linesConnector)
			delete linesConnector;
	}

	void turnTheCar(CarCtrl &carCtl){
		switch (getDirectionArg()) {
			case Path::TURNING_LEFT:
				carCtl.turnLeft();
				break;
			case Path::TURNING_RIGHT:
				carCtl.turnRight();
				break;
			default:
				// not turning action
				break;
		}
	}

	//================= only data =======================

	bool isBorderIdValid(int borderId){
		return (borderId < 4 && borderId > -1);
	}

	bool isPathValid(){
		return (startBorderId != -1 && destinationBorderId != -1);
	}

	void setCurrentBorder(int currentBorderId, CarCtrl &carctl) {
		if(currentBorderPos != currentBorderId){
			currentBorderPos = currentBorderId;
			if(carPosState != INVALID_STATE && currentBorderId > -1)
				// accept intersection state
				recognizeCarPosState(currentBorderId, carctl);
		}
		else{
//			cout<<"ERROR:current border id is not valid"<<endl;
			return;
		}
		if(startBorderId == -1 && isBorderIdValid(currentBorderId)){
			startBorderId = currentBorderId;
			carPosState	  = DEPARTURE;
		}
		if(!linesConnector)
			createConnector();
	}

	void setDestinationBorder(int destinationId) {
		// borderId should be >= 0 and <=3
		if(this->destinationBorderId == destinationId)
			return;
		if (isBorderIdValid(destinationId))
			this->destinationBorderId = destinationId;
		else
			cout<<"ERROR:destination border id is not valid"<<endl;
		if(!linesConnector)
			createConnector();
	}

	void setIntersection(Intersection *intersection){
		if(!this->intersection || this->intersection->getId() != intersection->getId())
			this->intersection = intersection;
	}

	int getCurrentBorderId(){
		return currentBorderPos;
	}

	int getDestinationBorderId(){
		return destinationBorderId;
	}

	int getStartBorderId() const {
		return startBorderId;
	}

	LinesConnector* getLinesConnector(){
		return linesConnector;
	}

	void setAreaChangeCb(void (*func_cb)(AreaChangeEvent event, Path *dispatcher, CarCtrl &ctrl)){
		areaChange_cb = func_cb;
	}

	TurningControl getDirectionArg() const {
		return directionArg;
	}

	PathState getCarPosState() const {
		return carPosState;
	}

	bool isBigTurn() const {
		return bigTurn;
	}

	void setBigTurn(bool bigTurn) {
		this->bigTurn = bigTurn;
	}
};




#endif /* SRC_MODEL_PATH_H_ */
