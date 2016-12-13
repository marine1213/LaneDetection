/*
 * LinesConnector.h
 *
 *  Created on: Nov 14, 2016
 *      Author: gnouchnam
 */

#ifndef LINESCONNECTOR_H_
#define LINESCONNECTOR_H_

#include "Lane.h"
#include "LinesInterface.h"

class LinesConnector : public LinesInterface{

public:
	LinesConnector(float *outGoingPts, float *inComingPts){
//		this->leftLine 	= new Line(outGoingPts[0], outGoingPts[1], inComingPts[0], inComingPts[1]);
		centerExLine 	= new Line(outGoingPts[2], outGoingPts[3], inComingPts[2], inComingPts[3]);
		leftExLine		= new Line(outGoingPts[4], outGoingPts[5], inComingPts[4], inComingPts[5]);
		rightExLine		= new Line(outGoingPts[6], outGoingPts[7], inComingPts[6], inComingPts[7]);
//		this->rightLine	= new Line(outGoingPts[8], outGoingPts[9], inComingPts[8], inComingPts[9]);
	}

};



#endif /* LINESCONNECTOR_H_ */
