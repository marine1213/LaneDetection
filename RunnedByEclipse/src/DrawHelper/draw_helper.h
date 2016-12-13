/*
 * draw_helper.h
 *
 *  Created on: Oct 18, 2016
 *      Author: gnouchnam
 */

#ifndef DRAW_HELPER_H_
#define DRAW_HELPER_H_

#include "../common.h"
#include "../Model/Lane.h"
#include "../Model/Lane.h"
#include "../Model/Car.h"
#include "../Model/IntersectionBorder.h"
#include "../Model/Intersection.h"
#include "../Model/LinesInterpolator.h"
#include "../Model/LinesConnector.h"
#include "../Model/PointSeries.h"

void drawPointsBuffer(PointSeries *buffer, Mat &img);

void drawConnectorLines(LinesConnector *cntor, Mat &img, Scalar *color = NULL);
void drawInterpolatedLines(LinesInterpolator *ipl, Mat &img);

void drawInterpolatedBorderLines(IntersectionBorder &ib, Mat &img);
void drawIntersectionBorder(IntersectionBorder &ib, Mat &img);

void drawCar(Car &car, Mat &img, vector<Scalar> vcolor);

void drawLine(Line &line, Mat &img, Scalar *color = NULL);

void drawLane(Lane &lane, Mat &img);
void drawLane(Lane &lane, Mat &img, Scalar *leftColor, Scalar *rightColor);

#endif /* DRAW_HELPER_H_ */
