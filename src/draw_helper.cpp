/*
 * draw_helper.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: gnouchnam
 */
#include "common.h"
#include "Model/Lane.h"
#include "Model/Lane.h"
#include "Model/Car.h"
#include "draw_helper.h"


void drawCar(Car &car, Mat &img, vector<Scalar> colorPt){
	Point plf = Point(car.t()[car.getLfId()], car.t()[car.getLfId()+1]);
	Point prf = Point(car.t()[car.getRfId()], car.t()[car.getRfId()+1]);
	Point plr = Point(car.t()[car.getLrId()], car.t()[car.getLrId()+1]);
	Point prr = Point(car.t()[car.getRrId()], car.t()[car.getRrId()+1]);
	Point pct = Point(car.getCenterPt()[0], car.getCenterPt()[1]);

	int idp = 0;
	circle(img,plf,2,colorPt[idp],3);
	circle(img,prf,2,Scalar(255,255,0),3);
	circle(img,plr,2,Scalar(255,0,255),3);
	circle(img,prr,2,Scalar(128,255,255),3);
	circle(img,pct,2,colorPt[idp],3);

	int idl = colorPt.size()>0?1:0;
	line(img, plf, prf,colorPt[idl], 2, CV_AA);
	line(img, prf, prr,colorPt[idl], 2, CV_AA);
	line(img, prr, plr,colorPt[idl], 2, CV_AA);
	line(img, plr, plf,colorPt[idl], 2, CV_AA);
}

void drawLane(Lane &lane, Mat &img){
	Scalar leftColor = Scalar(0,255,0);
	Scalar rightColor = Scalar(0,0,255);
	drawLane(lane,img,&leftColor, &rightColor);
}

void drawLane(Lane &lane, Mat &img, Scalar *leftColor, Scalar *rightColor){
	if(leftColor)
		drawLine(*(lane.getLeft()), img, *leftColor);
	if(rightColor)
		drawLine(*(lane.getRight()),img, *rightColor);
	Scalar centerColor = Scalar(196,0,196);
	drawLine(*(lane.getCenter()),img, centerColor);
}

void drawLine(Line &l, Mat &img){
	Scalar color = Scalar(0,196,196);
	drawLine(l,img,color);
}

void drawLine(Line &l, Mat &img, Scalar &color){
	Point pl1 = Point(l.getPoint()[0], l.getPoint()[1]);
	Point pl2 = Point(l.getPoint2()[0], l.getPoint2()[1]);
	line(img, pl1, pl2,color, 2, CV_AA);
}



