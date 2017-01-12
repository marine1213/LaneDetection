/*
 * testKalman.cpp
 *
 *  Created on: Dec 29, 2016
 *      Author: gnouchnam
 */

#include "KalmanAdapter.h"


bool getPt;
Point2f mousePos;

void clickEvent(int event, int x, int y, int flags, void* param) {
	if (getPt) {
		mousePos.x = x;
		mousePos.y = y;
		getPt = false;
		switch (event) {

		case EVENT_LBUTTONUP:

			break;
		case EVENT_RBUTTONDOWN:

			break;
		}
	}
}
void GetCursorPos(){
	getPt = true;
	while(getPt)
		waitKey(10);
}
int testKalman() {
	getPt = true;

	// Image to show mouse tracking
	Mat img(600, 800, CV_8UC3);
	imshow("mouse kalman", img);
	setMouseCallback("mouse kalman",clickEvent);

//	GetCursorPos();
	KalmanAdapter kAdapt;
//	kAdapt.init(mousePos.x, mousePos.y);

	vector<Point2f> mousev, kalmanv;
	mousev.clear();
	kalmanv.clear();

	while (1) {
		// Get mouse point
		GetCursorPos();
		Point2f statePt = kAdapt.update(mousePos.x,mousePos.y);

		Point measPt(mousePos.x, mousePos.y);

		mousev.push_back(measPt);
		kalmanv.push_back(statePt);

		for (unsigned int i = 0; i < mousev.size() - 1; i++)
			line(img, mousev[i], mousev[i + 1], Scalar(255, 255, 0), 1);

		for (unsigned int i = 0; i < kalmanv.size() - 1; i++)
			line(img, kalmanv[i], kalmanv[i + 1], Scalar(0, 155, 255), 1);
		imshow("mouse kalman", img);

		waitKey(10);
	}

	return 0;
}
