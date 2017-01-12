/*
 * draw_helper.h
 *
 *  Created on: Oct 18, 2016
 *      Author: gnouchnam
 */

#ifndef DRAW_HELPER_H_
#define DRAW_HELPER_H_

#include "../common.h"
#include "../Model/IntersectionElements/Lane.h"
#include "../Model/IntersectionElements/IntersectionBorder.h"
#include "../Model/IntersectionElements/Intersection.h"
#include "../Model/IntersectionElements/LinesInterpolator.h"
#include "../Model/IntersectionElements/LinesConnector.h"
#include "../Model/CarElements/PointSeries.h"
#include "../Model/CarElements/Car.h"

void drawPointsBuffer(PointSeries *buffer, Mat &img, Scalar color = Scalar(128,255,128));

void drawConnectorLines(LinesConnector *cntor, Mat &img, Scalar *color = NULL);
void drawInterpolatedLines(LinesInterpolator *ipl, Mat &img);

void drawInterpolatedBorderLines(IntersectionBorder &ib, Mat &img);
void drawIntersectionBorder(IntersectionBorder &ib, Mat &img);

void drawCar(Car &car, Mat &img, vector<Scalar> vcolor);

void drawLine(Line &line, Mat &img, Scalar *color = NULL);

void drawLane(Lane &lane, Mat &img);
void drawLane(Lane &lane, Mat &img, Scalar *leftColor, Scalar *rightColor);

#endif /* DRAW_HELPER_H_ */
