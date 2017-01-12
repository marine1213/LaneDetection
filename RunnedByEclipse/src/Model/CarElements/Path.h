/*
 * Path.h
 *
 *  Created on: Nov 28, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_MODEL_PATH_H_
#define SRC_MODEL_PATH_H_

#include "../../Controller/CarComm.h"


/**
 * Track the current position state of the car
 */
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

	void (*areaChange_cb)(AreaChangeEvent event, Path *dispatcher, CarComm &ctrl);

	int startBorderId, destinationBorderId, currentBorderPos;
	PathState 		carPosState;

	int laneId;

	//================= setup part ======================

	void recognizeCarPosState(int currentBorderId){
		if(currentBorderId == startBorderId){
			carPosState = DEPARTURE;
			if(areaChange_cb){
//				areaChange_cb(DEPATURE2TRANSIT, this, carctl);
			}
		}else if(currentBorderId == destinationBorderId){
			carPosState = ARRIVAL;
			if(areaChange_cb){
//				areaChange_cb(TRANSIT2ARRIVAL, this, carctl);
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
		carPosState			= INVALID_STATE;
		areaChange_cb		= NULL;
		laneId 				= -1;
	}



	//================= validating data =======================

	bool isBorderIdValid(int borderId){
		return (borderId < 4 && borderId > -1);
	}

	bool isPathValid(){
		return (startBorderId != -1 && destinationBorderId != -1);
	}

	bool isPositionValid(){
		return (getCurrentBorderId()!= -1 && getLaneId() != -1);
	}

	//============ complicate setter and getter ================
	void setCurrentBorder(int currentBorderId) {
		if(currentBorderPos != currentBorderId){
			currentBorderPos = currentBorderId;
			if(carPosState != INVALID_STATE && currentBorderId > -1)
				// accept intersection state
				recognizeCarPosState(currentBorderId);
		}
		else{
//			cout<<"ERROR:current border id is not valid"<<endl;
			return;
		}
		if(startBorderId == -1 && isBorderIdValid(currentBorderId)){
			startBorderId = currentBorderId;
			carPosState	  = DEPARTURE;
		}
	}

	void setDestinationBorder(int destinationId) {
		// borderId should be >= 0 and <=3
		if (destinationId == -1 || isBorderIdValid(destinationId))
			this->destinationBorderId = destinationId;
		else{
#if SHOW_PROCESSING_STATE
			cout<<"ERROR:destination border id is not valid"<<endl;
#endif
		}
	}


	TurningControl getDirectionArg() const {
		if(destinationBorderId == -1)
			return GOING_STRAIGHT;
		return (TurningControl)((destinationBorderId - startBorderId + 4) % 4);
	}

	//============== simple setter and getter ==================

	int getCurrentBorderId(){
		return currentBorderPos;
	}

	int getDestinationBorderId(){
		return destinationBorderId;
	}

	int getStartBorderId() const {
		return startBorderId;
	}

	int getLaneId() const {
		return laneId;
	}

	void setLaneId(int newLaneId){
		laneId = newLaneId;
	}

	void setAreaChangeCb(void (*func_cb)(AreaChangeEvent event, Path *dispatcher, CarComm &ctrl)){
		areaChange_cb = func_cb;
	}

	PathState getCarPathState() const {
		return carPosState;
	}
};




#endif /* SRC_MODEL_PATH_H_ */
