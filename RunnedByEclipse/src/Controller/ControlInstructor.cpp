/*
 * ControlInstructor.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: gnouchnam
 */
#include "ControlInstructor.h"

void areaChangeResponse(Path::AreaChangeEvent event, Path *path, CarCtrl &carCtrl){
	if(event == Path::DEPATURE2TRANSIT){
		carCtrl.setEventMsg("Go out of current lane");
//		if (path->getDirectionArg() == Path::TURNING_LEFT){
////			carCtrl.setKeepBalance(false);
//			carCtrl.turnLeft();
//		}
	}
//	if(event == Path::TRANSIT2ARRIVAL){
//		path->setBigTurn(false);
//	}
}

void sideStateResponse(Car::SideStateEvent event, Car *dispatcher, CarCtrl &carCtl){
	switch (event) {
		case Car::RIGHT_LINE_WARNING:
			if(carCtl.isWorking)
				carCtl.moveLittleLeft();
			break;
		case Car::LEFT_LINE_WARNING:
			if(carCtl.isWorking)
				carCtl.moveLittleRight();
			break;
		default:
			break;
	}
}

bool stopConfirmation;
clock_t stoppedTimer;

void frontStateResponse(Car::FrontStateEvent event, Car *dispatcher,CarCtrl &carCtl) {
	switch (event) {
	case Car::NEAR_STOP_LINE:
		carCtl.setEventMsg("Near stop line event");
		if (carCtl.isWorking && dispatcher->isStopAtFirstStopLine()) {
//			carCtl.stop();
//			carCtl.stop();
//			carCtl.stop();
//			if (!dispatcher->isMoving()) {
//				if (!stopConfirmation) {
//					stoppedTimer = clock();
//					stopConfirmation = true;
//				} else {
//					double timePassed = (double) (clock() - stoppedTimer) / CLOCKS_PER_SEC;
//					if (timePassed > 3) {
//						carCtl.haltCurrentWork();
//						stopConfirmation = false;
//						dispatcher->setStopAtFirstStopLine(false);
//						carCtl.go();
//					}
//				}
//			}
		}
		break;
	case Car::ON_STOP_LINE:{
		carCtl.setEventMsg("On stop line event");
		// Turn the car right here
		Path::TurningControl direction = dispatcher->getPath().getDirectionArg();
		if (direction == Path::TURNING_LEFT || direction == Path::TURNING_RIGHT){
			dispatcher->getPath().setBigTurn(true);
			if(direction == Path::TURNING_LEFT )
				carCtl.setTurnLeftAngle();
			if(direction == Path::TURNING_RIGHT )
				carCtl.setTurnRightAngle();
		}
//		if (dispatcher->getPath().getDirectionArg() == Path::TURNING_RIGHT) {
//			std::cout << "Turn right" << std::endl;
//			carCtl.setEventMsg("TurnRight");
////			carCtl.setKeepBalance(false);
//		}
		break;
	}
	default:
		break;
	}
}

