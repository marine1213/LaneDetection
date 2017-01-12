/*
 * EventDetectionThread.h
 *
 *  Created on: Jan 5, 2017
 *      Author: gnouchnam
 */

#ifndef SRC_THREADS_EVENTDETECTIONTHREAD_H_
#define SRC_THREADS_EVENTDETECTIONTHREAD_H_

#include "../constant.h"
#include "../System/SubThread.h"
#include "../Controller/CarCtrl.h"

class EventDetectionThread : public SubThread{
public:
	EventDetectionThread(CarCtrl *sourceCtl){
		carCtl 			= sourceCtl;
		ipl 			= NULL;
		triggerLine		= NULL;
		turningState	= false;
		stoppedTimer 	= clock();
		stopConfirmation = false;
		stopAtFirstLine = false;
	};

	void startThread(){
		triggerLine 	  = carCtl->getTurningLine();
		centerPtBuffer[0] = carCtl->getData()->getCarData()->getCenterPt()[0];
		centerPtBuffer[1] = carCtl->getData()->getCarData()->getCenterPt()[1];
		this->exec();
	}

private:
	CarCtrl *carCtl;
	Line	*triggerLine;
	float	centerPtBuffer[2];
	LinesInterface	*ipl;

	bool	turningState;

	bool stopConfirmation;
	clock_t stoppedTimer;
	bool stopAtFirstLine;

	void content(){
		if(!carCtl->getDriveThread()->isRunning())
			return;
		Constant::ControlEvent event = Constant::INITIALIZED;
		//TODO: Emergency or Stop the car
//		if(stopAtFirstLine){
//			int currentBorderId = carCtl->getPath().getCurrentBorderId();
//			if (carCtl->getPath().isBorderIdValid(currentBorderId)) {
//				Lane *stopLane =
//						carCtl->getData()->getIntersection()->getBorders()[currentBorderId]->getStopLane();
//				//			float dStop = getMinDistanceOf4pts(stopLane->getCenter());
//				float dStop = stopLane->getCenter()->distance(
//						carCtl->getData()->getCarData()->getCenterPt()[0],
//						carCtl->getData()->getCarData()->getCenterPt()[1]);
//#if CHECK_CENTER_PT
//				cout<<"carData->getCenterPt(): "<<carData->getCenterPt()[0]<<" - "<<carData->getCenterPt()[1]<<endl;
//#endif
//#if CHECK_DISTANCE2STOPLINE
//				if(abs(dStop) >0)
//				cout<<"To stop line = "<<dStop<<endl;
//#endif
//				//TODO-bug triggered by other objects
//				if(carCtl->getDriveThread()->isRunning() && stopAtFirstLine){
//
//					float wheelBase = carCtl->getData()->getCarData()->getWheelBase();
//					if (dStop < wheelBase*1.9 && currentBorderId == carCtl->getPath().getStartBorderId()) {
//						carCtl->getDriveThread()->input(event = Constant::STOP_EVENT);
//						if (!carCtl->getData()->getCarData()->isMoving()) {
//							if (!stopConfirmation) {
//								stoppedTimer = clock();
//								stopConfirmation = true;
//								carCtl->getData()->getCarData()->keepWheelBase();
//							} else {
//								float timePassed = (float) (clock()- stoppedTimer) / CLOCKS_PER_SEC;
//								cout<<"Time passed = "<<timePassed<<endl;
//								if (timePassed > 3) {
//									carCtl->getDriveThread()->input(event = Constant::START_EVENT);
//									stopConfirmation = false;
//									stopAtFirstLine = false;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
		cout << "Event - turn" << endl;
		if(!triggerLine)
			cout<<"stop"<<endl;
		if (triggerLine && event == Constant::INITIALIZED) {
			// turning when the car stays near trigger lines
			// distance from center point to trigger turning line
			float dTrigger = triggerLine->distance(centerPtBuffer[0],centerPtBuffer[1]);

			float wheelBase = carCtl->getData()->getCarData()->getWheelBase();
			cout << "D trigger = " << dTrigger << "wb = "<<wheelBase<<endl;
			if (dTrigger < 195 /*195*/ && carCtl->getDirection() == Path::TURNING_LEFT) {
				carCtl->getDriveThread()->input(event = Constant::TURN_LEFT_EVENT);
				turningState = true;
			} else if (dTrigger < wheelBase*2.088/*188*/ && carCtl->getDirection() == Path::TURNING_RIGHT) {
				carCtl->getDriveThread()->input(event = Constant::TURN_RIGHT_EVENT);
				turningState = true;
			}
			if (turningState) {
				// stop turning
//				float prevAngle = carCtl->getData()->getCarData()->getRelativeAngle();
				// third - calculate angle here
				carCtl->angleToNextCenterLine();
//				cout << "D angle = "<< (carCtl->getData()->getCarData()->getRelativeAngle()- prevAngle) << endl;
				if (carCtl->getData()->getCarData()->getRelativeAngle() < 2) {
					turningState = false;
					carCtl->getPath().setDestinationBorder(-1);
				}
			}
		}
		if (!turningState && event == Constant::INITIALIZED) {
			// if the car state is not turning, then
			// keep balance on straight line
			ipl = carCtl->getIplines();
			if (ipl) {
				// calculate distance
				float balanceDistance = ipl->getCenterExLine()->distance(centerPtBuffer[0], centerPtBuffer[1]);
				float ctDistLeft = ipl->getLeftExLine()->distance(centerPtBuffer[0], centerPtBuffer[1], true);
				float ctDistRight = ipl->getRightExLine()->distance(centerPtBuffer[0], centerPtBuffer[1], true);

				// turning direction selection
				int leftOrRight = abs(ctDistRight) > abs(ctDistLeft) ? -1 : 1;
				int insideOrOut = signbit(ctDistRight * ctDistLeft);
				ctDistLeft = insideOrOut ? abs(ctDistLeft) : -abs(ctDistLeft);
				ctDistRight = insideOrOut ? abs(ctDistRight) : -abs(ctDistRight);
#if CHECK_INSIDE_OR_OUTSIDE_OF_IPL
				cout << "inside = "<< insideOrOut<<endl;
#endif
				if (balanceDistance > 0) {
					if (leftOrRight == -1) { // && ctDistLeft <= 5) {
						carCtl->getDriveThread()->input(event = Constant::BALANCE_LEFT_EVENT);
#if SHOW_PROCESSING_STATE
						cout << "Warning left lane: Dleft="<<ctDistLeft<<" Dright="<<ctDistRight<< endl;
#endif
					}
					if (leftOrRight == 1) {		// && ctDistRight <= 5) {
						carCtl->getDriveThread()->input(event = Constant::BALANCE_RIGHT_EVENT);
#if SHOW_PROCESSING_STATE
						cout << "Warning right lane: Dleft="<<ctDistLeft<<" Dright="<<ctDistRight<< endl;
#endif
					}
					// not consider outer area yet
				}
			}
		}

	}
};



#endif /* SRC_THREADS_EVENTDETECTIONTHREAD_H_ */
