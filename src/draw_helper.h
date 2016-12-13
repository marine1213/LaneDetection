/*
 * draw_helper.h
 *
 *  Created on: Oct 18, 2016
 *      Author: gnouchnam
 */

#ifndef DRAW_HELPER_H_
#define DRAW_HELPER_H_

void drawCar(Car &car, Mat &img, vector<Scalar> vcolor);

void drawLine(Line &line, Mat &img);
void drawLine(Line &line, Mat &img, Scalar &color);

void drawLane(Lane &lane, Mat &img);
void drawLane(Lane &lane, Mat &img, Scalar *leftColor, Scalar *rightColor);

#endif /* DRAW_HELPER_H_ */
