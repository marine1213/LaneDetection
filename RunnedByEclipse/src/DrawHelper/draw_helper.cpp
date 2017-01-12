/*
 * draw_helper.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: gnouchnam
 */


#include "draw_helper.h"

void drawPointsBuffer(PointSeries *buffer, Mat &img, Scalar color){
	for (int i = 0; i < buffer->size(); ++i) {
		Point pt = Point(buffer->get(i)[0], buffer->get(i)[1]);
		circle(img,pt,2,color,3);
	}
}

void drawConnectorLines(LinesConnector *cntor, Mat &img, Scalar *color){
//	drawLine(*cntor->getLeftLine(), img, color);
//	drawLine(*cntor->getRightLine(), img, color);
	drawLine(*cntor->getCenterExLine(), img, color);
	ostringstream ss;
	ss<< cntor->getCenterExLine()->getAngle();
	float *pt = cntor->getCenterExLine()->getPoint();
	putText(img, ss.str(), Point(pt[0], pt[1]), FONT_HERSHEY_COMPLEX, 0.5,Scalar(255, 255, 255), 1, CV_AA);
	drawLine(*cntor->getLeftExLine(),img, color);
	drawLine(*cntor->getRightExLine(),img, color);
}

void drawInterpolatedLines(LinesInterpolator *ipl, Mat &img){
//	drawLine(*ipl->getLeftLine(), img);
//	drawLine(*ipl->getRightLine(), img);
	Scalar centerLineColor = Scalar(255,255,255);
	drawLine(*ipl->getCenterExLine(), img, &centerLineColor);
	Scalar leftLineColor =  Scalar(0,255,255);
	drawLine(*ipl->getLeftExLine(),img, &leftLineColor);
	Scalar rightLineColor = Scalar(255,255,0);
	drawLine(*ipl->getRightExLine(),img, &rightLineColor);
}

void drawInterpolatedBorderLines(IntersectionBorder &ib, Mat &img){
	for (int i = 0; i < 2; ++i) {
		drawInterpolatedLines(ib.getIpl()[i],img);
	}
}

void drawIntersectionBorder(IntersectionBorder &ib, Mat &img){
	if(ib.getLeftLane())
		drawLane(*ib.getLeftLane(),img);
	if(ib.getRightLane())
		drawLane(*ib.getRightLane(),img);
	if(ib.getYellowLane())
		drawLane(*ib.getYellowLane(),img);
	if(ib.getStopLane())
		drawLane(*ib.getStopLane(),img);
}

void drawCar(Car &car, Mat &img, vector<Scalar> colorPt){
	if(car.getTires().size() == 5){
		Point plf = Point(car.getTires()[4].x, car.getTires()[4].y);
		Point prf = Point(car.getTires()[1].x, car.getTires()[1].y);
		Point plr = Point(car.getTires()[2].x, car.getTires()[2].y);
		Point prr = Point(car.getTires()[3].x, car.getTires()[3].y);

		circle(img,plf,2,Scalar(255,255,255),3);
		circle(img,prf,2,Scalar(255,255,255),3);
		circle(img,plr,2,Scalar(255,255,255),3);
		circle(img,prr,2,Scalar(255,255,255),3);
	}
	int idp = 0;

	Point pct = Point(car.getCenterPt()[0], car.getCenterPt()[1]);
	circle(img,pct,2,colorPt[idp],3);

//	int idl = colorPt.size()>0?1:0;
//	line(img, plf, prf,colorPt[idl], 2, CV_AA);
//	line(img, prf, prr,colorPt[idl], 2, CV_AA);
//	line(img, prr, plr,colorPt[idl], 2, CV_AA);
//	line(img, plr, plf,colorPt[idl], 2, CV_AA);
}

void drawLane(Lane &lane, Mat &img){
	Scalar leftColor = Scalar(0,255,0);
	Scalar rightColor = Scalar(0,0,255);
	drawLane(lane,img,&leftColor, &rightColor);
}

void drawLane(Lane &lane, Mat &img, Scalar *leftColor, Scalar *rightColor){
	if(leftColor)
		drawLine(*(lane.getLeft()), img, leftColor);
	if(rightColor)
		drawLine(*(lane.getRight()),img, rightColor);
	Scalar centerColor = Scalar(196,0,196);
	drawLine(*(lane.getCenter()),img, &centerColor);
}

void drawLine(Line &l, Mat &img, Scalar *color){
	Scalar tempColor = Scalar(0,196,196);
	if(!color)
		color = &tempColor;
	Point pl1 = Point(l.getPoint()[0], l.getPoint()[1]);
	Point pl2 = Point(l.getPoint2()[0], l.getPoint2()[1]);
	line(img, pl1, pl2,*color, 1, CV_AA);
}



