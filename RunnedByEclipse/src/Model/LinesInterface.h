/*
 * LinesInterface.h
 *
 *  Created on: Nov 23, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_MODEL_LINESINTERFACE_H_
#define SRC_MODEL_LINESINTERFACE_H_

class LinesInterface {
protected:
	Line *leftExLine, *centerExLine, *rightExLine;
	Line *leftLine, *rightLine;
public:

	LinesInterface(){
		leftExLine = centerExLine = rightExLine = leftLine = rightLine = NULL;
	}

	~LinesInterface(){
		if(rightLine)
			delete rightLine;
		if(leftLine)
			delete leftLine;
		if(leftExLine)
			delete leftExLine;
		if(centerExLine)
			delete centerExLine;
		if(rightExLine)
			delete rightExLine;
	}

	bool isValid(){
		if(!centerExLine || !leftExLine || !rightExLine)
			return false;
		return true;
	}

	Line* getLeftLine() {
		return leftLine;
	}

	Line* getRightLine() {
		return rightLine;
	}

	Line* getCenterExLine() {
		return centerExLine;
	}

	Line* getLeftExLine() {
		return leftExLine;
	}

	Line* getRightExLine() {
		return rightExLine;
	}
};




#endif /* SRC_MODEL_LINESINTERFACE_H_ */
