/*
 * my_tools.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: gnouchnam
 */

#include "my_tools.h"

Mat m;

double thres = 1, upthres = 16;
void hueSelection(vector<Mat> inputHLS) {
	// extract yellow area
	char c = 'a';

	while (c != 27) {
		c = waitKey(10);
		if (c == 'b')
			thres += (thres < 255) ? 1 : 0;
		if (c == 'c')
			thres -= (thres > 1) ? 1 : 0;
		if (c == 'g')
			upthres += (upthres < 255) ? 1 : 0;
		if (c == 'd')
			upthres -= (upthres > 1) ? 1 : 0;

		cout << "[" << thres << "] -> [" << upthres << "]\n";
		Mat yellowMask, yellowImg;
		inRange(inputHLS[0], Scalar(thres), Scalar(upthres), yellowMask);
		imshow("yellowMask", yellowMask);

	}
}
void mouseHandler(int event, int x, int y, int flags, void* param) {
	int c = m.at<uchar>(y, x);
	cout << c << endl;
	switch (event) {

	case EVENT_LBUTTONUP:
		cout << c << endl;
		break;
	}
}

void colorPixelPicker(Mat &input, string name) {
	m = input.clone();
	imshow("color picker" + name, input);
	setMouseCallback("color picker" + name, mouseHandler);
}
