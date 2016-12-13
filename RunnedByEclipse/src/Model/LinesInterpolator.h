/*
 * LinesInterpolator.h
 *
 *  Created on: Nov 14, 2016
 *      Author: gnouchnam
 */

#ifndef LINESINTERPOLATOR_H_
#define LINESINTERPOLATOR_H_

static const unsigned char 	BORDER_LEFT 		= 0;
static const unsigned char  BORDER_TOP 			= 1;
static const unsigned char 	BORDER_RIGHT 		= 2;
static const unsigned char 	BORDER_BOTTOM 		= 3;
static const unsigned char 	INTERSECTION 		= 4;	// to indicate the point lies within center of intersection

// Ids are depended by findMyInterpolatedLines/car.h
static const unsigned char 	IPLS_OUTPUT 		= 1;
static const unsigned char 	IPLS_INPUT 			= 0;

#include "Line.h"
#include "Lane.h"
#include "LinesInterface.h"

class LinesInterpolator : public LinesInterface{
private:
	int borderId;
	bool inversed;

public:
	LinesInterpolator(Line *leftLine, Line *rightLine,int borderId = -1){
		this->borderId	= borderId;
		this->leftLine 	= leftLine;
		this->rightLine	= rightLine;
		centerExLine 	= leftLine->findMidLine(*(rightLine),1.0/2);
		rightExLine		= leftLine->findMidLine(*(rightLine),3.0/8);
		leftExLine		= leftLine->findMidLine(*(rightLine),5.0/8);
		inversed		= false;
	}

	float* getPtsInIntersection(){
		float *pts = new float[10];
		switch (borderId) {
			case BORDER_TOP:
			case BORDER_LEFT:
				pts[0] = getLeftLine()->getPoint2()[0];
				pts[1] = getLeftLine()->getPoint2()[1];
				pts[2] = getLeftExLine()->getPoint2()[0];
				pts[3] = getLeftExLine()->getPoint2()[1];
				pts[4] = getCenterExLine()->getPoint2()[0];
				pts[5] = getCenterExLine()->getPoint2()[1];
				pts[6] = getRightExLine()->getPoint2()[0];
				pts[7] = getRightExLine()->getPoint2()[1];
				pts[8] = getRightLine()->getPoint2()[0];
				pts[9] = getRightLine()->getPoint2()[1];
				return pts;
			case BORDER_RIGHT:
			case BORDER_BOTTOM:
				pts[0] = getLeftLine()->getPoint()[0];
				pts[1] = getLeftLine()->getPoint()[1];
				pts[2] = getLeftExLine()->getPoint()[0];
				pts[3] = getLeftExLine()->getPoint()[1];
				pts[4] = getCenterExLine()->getPoint()[0];
				pts[5] = getCenterExLine()->getPoint()[1];
				pts[6] = getRightExLine()->getPoint()[0];
				pts[7] = getRightExLine()->getPoint()[1];
				pts[8] = getRightLine()->getPoint()[0];
				pts[9] = getRightLine()->getPoint()[1];
				return pts;
			default:
				delete pts;
				assert("Wrong border id - pts of intersection" == NULL);
				break;
		}
		return NULL;
	}

	void setLimitedArea(int left, int top, int right, int bot, Mat *img = NULL, float *exceptionData = NULL){
		centerExLine->setLimitedArea(left,top,right,bot,img,exceptionData);
		leftExLine->setLimitedArea(left,top,right,bot,img,exceptionData);
		rightExLine->setLimitedArea(left,top,right,bot,img,exceptionData);
	}

	void inverseOrder(){
		inversed = true;
		Line * tempLine;
		tempLine = leftLine;
		leftLine = rightLine;
		rightLine = tempLine;
		tempLine 	= leftExLine;
		leftExLine	= rightExLine;
		rightExLine	= tempLine;
	}

	bool isInversered(){
		return inversed;
	}
};


#endif /* LINESINTERPOLATOR_H_ */
