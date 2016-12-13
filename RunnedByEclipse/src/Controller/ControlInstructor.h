/*
 * ControlInstructor.h
 *
 *  Created on: Dec 3, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_CONTROLLER_CONTROLINSTRUCTOR_H_
#define SRC_CONTROLLER_CONTROLINSTRUCTOR_H_


#include "CarCtrl.h"
#include "../Model/Car.h"
#include "../Model/Path.h"
#include "time.h"
#include "iostream"


void areaChangeResponse(Path::AreaChangeEvent event, Path *dispatcher, CarCtrl &ctrl);

void sideStateResponse(Car::SideStateEvent event, Car *dispatcher, CarCtrl &ctrl);

void frontStateResponse(Car::FrontStateEvent event, Car *dispatcher, CarCtrl &ctrl);

#endif /* SRC_CONTROLLER_CONTROLINSTRUCTOR_H_ */
