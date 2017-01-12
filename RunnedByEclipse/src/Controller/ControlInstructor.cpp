/*
 * ControlInstructor.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: gnouchnam
 */
#include "ControlInstructor.h"
#include "iostream"

//void areaChangeResponse(Path::AreaChangeEvent event, Path *path, CarComm &carComm){
//	if(event == Path::DEPATURE2TRANSIT){
////		if (path->getDirectionArg() == Path::TURNING_LEFT){
//////			carCtrl.setKeepBalance(false);
////			carCtrl.turnLeft();
////		}
//	}
////	if(event == Path::TRANSIT2ARRIVAL){
////		path->setBigTurn(false);
////	}
//}

//void sideStateResponse(CarCtrl::SideStateEvent event, Car *dispatcher, CarComm &carComm){
//	switch (event) {
//		case CarCtrl::RIGHT_LINE_WARNING:
//			if(carCtl.isMovingPeriod())
//				carCtl.moveLittleLeft();
//			break;
//		case CarCtrl::LEFT_LINE_WARNING:
//			if(carCtl.isMovingPeriod())
//				carCtl.moveLittleRight();
//			break;
//		default:
//			break;
//	}
//}

//bool stopConfirmation = false;
//clock_t stoppedTimer;
//
//void frontStateResponse(CarCtrl::FrontStateEvent event, Car *dispatcher,CarComm &carComm) {
//	switch (event) {
//	case CarCtrl::NEAR_STOP_LINE:{
////		carCtl.setEventMsg("Near stop line event");
////		std::cout<<("Near stop line event")<<endl;
////		Path::TurningControl direction = dispatcher->getPath().getDirectionArg();
////		if (carCtl.isRunning() && dispatcher->isStopAtFirstStopLine()) {
////			carCtl.setMvCtrlStt(CarCtrl::STOP);
////
//////			if (direction == Path::TURNING_LEFT || direction == Path::TURNING_RIGHT)
//////						dispatcher->getPath().initTurningState();
////
////			if (!dispatcher->isMoving()) {
////				if (!stopConfirmation) {
////					stoppedTimer = clock();
////					std::cout<<"mile stone:"<<stoppedTimer<<endl;
////					stopConfirmation = true;
////					carCtl.setMvCtrlStt(CarCtrl::READY);
////					carCtl.stopMovingPeriod();
////
////				} else {
////					double timePassed = (double) (clock() - stoppedTimer) / CLOCKS_PER_SEC;
////					std::cout<<"time passed:"<<timePassed<<endl;
////					if (timePassed > 3) {
////						carCtl.goStraight();
////						stopConfirmation = false;
////						dispatcher->setStopAtFirstStopLine(false);
////
////						carCtl.go();
////
////						switch (dispatcher->getPath().getDirectionArg()) {
////						case Path::GOING_STRAIGHT:
////							carCtl.setMvCtrlStt(CarCtrl::KEEP_BALANCE);
////							break;
////						case Path::TURNING_LEFT:
////						case Path::TURNING_RIGHT:
////							if (direction == Path::TURNING_LEFT)
////								carCtl.setTurnLeftAngle();
////							if (direction == Path::TURNING_RIGHT)
////								carCtl.setTurnRightAngle();
////							carCtl.setMvCtrlStt(CarCtrl::BIG_TURNING);
////							break;
////						default:
////							// be careful with bugs here
////							break;
////						}
////					}
////				}
////			}
////		}
////		else{
//
//		break;
//	}
//	case CarCtrl::ON_STOP_LINE:{
////		carCtl.setEventMsg("On stop line event");
//		break;
//	}
//	default:
//		break;
//	}
//}

