/*
 * img_processing.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: gnouchnam
 */
#include "img_processing.h"

#include "DrawHelper/draw_helper.h"



void laneDetection(Mat &src, LaneColor laneType, Data &data);
void erase1PixelOfBorders(Mat &input, Mat &output);

void perspectiveTransform(Mat &input, Mat &output);
void select_points(int event, int x, int y, int flags, void* userdata);
void selectLaneCandidates(vector<Line> *reconsideredCnt,vector<Line> &lineInOneCnt);


void perspectiveTransform(Mat &input, Mat &output){
//	/get4Corners(input);
	input.copyTo(output);
}
/**
 * ppMap is archived from here
 */
void setupPPMap(Mat &input,Mat &transformMat, Mat & ppmap){
	Mat ppSetLayer;
	input.copyTo(ppSetLayer);

	Point2f p4i[4],map_p[4];
	p4i[0] = Point2f();
	imshow("Perspective Settings",ppSetLayer);

	for(int i = 0; i<4;){

		setMouseCallback("Perspective Settings",select_points,&p4i[i]);


		if( p4i[i].x>0 && p4i[i].y>0 ){
			if(i>0)
				line(ppSetLayer,Point(p4i[i].x,p4i[i].y),Point(p4i[i-1].x,p4i[i-1].y),Scalar(0,0,255),2);
			if(i==3)
				line(ppSetLayer,Point(p4i[0].x,p4i[0].y),Point(p4i[i].x,p4i[i].y),Scalar(0,0,255),2);
			circle(ppSetLayer,Point(p4i[i].x,p4i[i].y),2,Scalar(0,255,0),2);
			imshow("Perspective Settings",ppSetLayer);
			i++;
			if(i<4)
				p4i[i] = Point2f();
		}
		waitKey(1);
	}

	map_p[0] = Point2f(0, 0);
	map_p[1] = Point2f(MAP_WIDTH, 0);
	map_p[2] = Point2f(MAP_WIDTH, MAP_HEIGHT);
	map_p[3] = Point2f(0, MAP_HEIGHT);

	transformMat = getPerspectiveTransform(p4i,map_p);
	ppmap = Mat::zeros(input.size(), CV_8UC3);
	warpPerspective(input, ppmap, transformMat, Size(MAP_WIDTH, MAP_HEIGHT));
}

void select_points(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN){//&& flags == EVENT_FLAG_CTRLKEY
//		cout << "Left button clicked pos x: " << x << ", pos y: " << y << endl;
		Point2f *p = (Point2f *)userdata;
		p->x = x;
		p->y = y;
	}
}

//=============================================================================
void laneProcessing(Mat &input, int imgId,Data &data) {
	cout<<"lane processing"<<endl;
	if (imgId) {
		//-------Delete 1/3 upper area of image to remove sky-----
		//	int newRow = input.rows * 1.0 / 3;
		//	rectangle(cleanedInput, Point(0, 0), Point(input.cols - 1, newRow),
		//			Scalar(0, 0, 0), CV_FILLED);

		//---------------------color filter-----------------------
		Mat hlsImg;
		cvtColor(input, hlsImg, CV_BGR2HLS);

//		vector<Mat> hsvChannels;
//		split(hlsImg, hsvChannels);
//		colorPixelPicker(hsvChannels[0],"H picker");
//		imshow("L",hsvChannels[1]);
//		colorPixelPicker(hsvChannels[2],"S picker");

		Mat yellowMask, whiteMask;		// blackMaskHls;
		inRange(hlsImg, Scalar(10, 0, 100), Scalar(30, 255, 255), yellowMask);
		inRange(hlsImg, Scalar(0, 200, 0), Scalar(255, 255, 255), whiteMask);
//		inRange(hlsImg, Scalar(0, 65, 0), Scalar(255, 145, 255), blackMaskHls);  // to get black area
#if DEBUG
		imshow("yellow",yellowMask);
		imshow("white",whiteMask);
#endif

		//----------------extract lanes----------------------

		laneDetection(whiteMask, WHITE_LANE, data);
		laneDetection(yellowMask, YELLOW_LANE, data);

		//---------interpolate centerLine-----------------------
		data.intersection->interpolateLines(&data.tmpDetectionImg);
	}
}

void laneDetection(Mat &src, LaneColor laneColor, Data &data) {
	cout<<"lane detection"<<endl;
	erase1PixelOfBorders(src,src);
	// prepare for close operator
	Mat  morphSrc;
//	Mat elmClose = getStructuringElement(CV_SHAPE_ELLIPSE, Size(3,3), Point(1, 1));
	Mat elmOpen = getStructuringElement(CV_SHAPE_ELLIPSE, Size(9,9), Point(4, 4));

	ostringstream str;
	str<<(laneColor==WHITE_LANE?"White lane":"Yellow lane");

//	morphologyEx(src, morphSrc, MORPH_CLOSE, elmClose);
	morphologyEx(src, morphSrc, MORPH_OPEN, elmOpen); // close to fill the gaps in thick lane

//	imshow(str.str()+" morphed", morphSrc);

	// prepare for contour operator
	vector<vector<Point> > cnts;
	vector<Vec4i> hier;
	/// Find contours
	findContours(morphSrc, cnts, hier, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	/// Draw contours
	Mat empty = Mat::zeros(src.size(), CV_8UC1);
	Mat draw = Mat::zeros(src.size(), CV_8UC3);

	for (size_t i = 0; i < cnts.size(); i++) {
		double cntSize = contourArea(cnts[i]);
		Rect rect = boundingRect(cnts[i]);
		if(cntSize<15)
			continue;
		Mat drClone = empty.clone();
		drawContours(drClone, cnts, i, Scalar(1), 1, 4, hier, 0, Point());
		drawContours(draw, cnts, i, Scalar(128, 128, 255), 1, 4, hier, 0, Point());

		// find Hough lines
		vector<Vec4i> lines;
		vector<Line> lineInOneCnt;
		vector<Line> reconsideredCnt[4];
		HoughLinesP(drClone, lines, 1, CV_PI / 180, 30, 30, 10);
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			Line tempLine = Line(l[0], l[1], l[2], l[3]);

			bool isNew = true;
			for (size_t j = 0; j < lineInOneCnt.size(); ++j) {
				if (lineInOneCnt[j].isOn(tempLine)) {
					isNew = false;
					reconsideredCnt[j].push_back(tempLine);
					break;
				}
			}
			if (isNew)
				lineInOneCnt.push_back((tempLine));
		}
		selectLaneCandidates(reconsideredCnt,lineInOneCnt);

		float l[4];
		for (size_t i = 0; i < lineInOneCnt.size(); ++i) {
			l[0] = lineInOneCnt[i].getPoint()[0];
			l[1] = lineInOneCnt[i].getPoint()[1];
			l[2] = lineInOneCnt[i].getPoint2()[0];
			l[3] = lineInOneCnt[i].getPoint2()[1];
			line(draw, Point(l[0], l[1]), Point(l[2], l[3]),Scalar(0, 255, 0), 1, CV_AA);
			ostringstream ss;
			ss << lineInOneCnt[i].getAngle();
			putText(draw, ss.str(), Point(l[0], l[1]+10*i), FONT_HERSHEY_COMPLEX, 0.85,Scalar(255, 255, 255), 1, CV_AA);
		}


		// find border the lane lines belong to
		int borderId = data.intersection->findDenpendedBorder(rect);

		// contour with 2 lines contains left lane line or middle line
		if(lineInOneCnt.size() == 2){

			Lane *lane = new Lane(new Line(lineInOneCnt[0]), new Line(lineInOneCnt[1]),laneColor);

			// discriminate left lane line or middle lane line by color
			if(lane->getColor() == YELLOW_LANE){

				// yellow line is feature of middle line
				cout<<"add yellow"<<endl;
				lane->setType(LANE_MID);
			}
			else if(lane->getColor() == WHITE_LANE){

				// white line is feature of left line
				cout<<"add left"<<endl;
				lane->setType(LANE_LEFT);
			}

			// add the lane to intersection
			data.intersection->addLane(lane,borderId,&rect);
			rectangle(draw,rect,Scalar(0,0,255),4);
		}

		// contour with 4 lines consists of right line and stop line
		if(lineInOneCnt.size() == 4){
			data.intersection->recognizeStopLine(lineInOneCnt,borderId, &rect);
			rectangle(draw,rect,Scalar(0,255,255),4);
		}
#if DEBUG
		imshow(str.str()+" show cnts", draw);
#endif
	}
}

void selectLaneCandidates(vector<Line> *reconsideredCnt,vector<Line> &lineInOneCnt){
	cout<<"select lane candidates"<<endl;
	for (size_t i = 0; i < lineInOneCnt.size(); ++i) {

		float minAngle = lineInOneCnt[i].getAngle();
		minAngle = minAngle>90?180-minAngle:minAngle;
		float desiredAngle = minAngle<10?0:90;

		int replacedId = -1;
		for (size_t j = 0; j < reconsideredCnt[i].size(); ++j) {
			float deltaAngle = abs(desiredAngle - reconsideredCnt[i][j].getAngle());
			if(deltaAngle < minAngle){
				minAngle = deltaAngle;
				replacedId = j;
			}
		}
		if(replacedId > -1)
			lineInOneCnt[i] = reconsideredCnt[i][replacedId];
	}
}

void erase1PixelOfBorders(Mat &input, Mat &output){
	cout<<"erase pixel on the border"<<endl;
	input.copyTo(output);
	for (int i = 0; i < input.cols; ++i) {
		output.at<char>(input.rows - 1,i) = 0;
		output.at<char>(0,i) = 0;
	}
	for (int i = 0; i < input.rows; ++i) {
		output.at<char>(i,input.cols - 1) = 0;
		output.at<char>(i,0) = 0;
	}
}

void mvoDetection(Mat &img,Data &outputData);

Mat elm = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));

void mvoFilter(Mat &input, Mat &output,Data &data){
	// pre processing
	Mat grayInput;
	cvtColor(input, grayInput, COLOR_BGR2GRAY);
	data.pMOG2->apply(grayInput, output,0.0001);
	morphologyEx(output, output, MORPH_CLOSE, elm);

	imshow("mov obj thres close",output);

	// mvo detection
	mvoDetection(output,data);
}

void mvoDetection(Mat &img,Data &data){
	vector<vector<Point> > cnts;
	vector<Vec4i> hier;
	/// Find contours
	findContours(img, cnts, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	/// Draw contours
	//vector<Line> vLine;			// Including all lines, to remove existed line

	for (size_t i = 0; i < cnts.size(); i++) {
		float area = contourArea(cnts[i]);
		if(area>100){
			//drawContours(data.tmpMvoImg, cnts, i, Scalar(0,255,255), 1, 4, hier, 0, Point());
			RotatedRect rrect = minAreaRect(cnts[i]);
			Point2f vPt[4]; rrect.points( vPt );

			vector<Point2f> cameraPt;
			cameraPt.push_back(Point2f(vPt[0].x,vPt[0].y));
			cameraPt.push_back(Point2f(vPt[1].x,vPt[1].y));
			cameraPt.push_back(Point2f(vPt[2].x,vPt[2].y));
			cameraPt.push_back(Point2f(vPt[3].x,vPt[3].y));
			cameraPt.push_back(Point2f(rrect.center.x,rrect.center.y));
			vector<Point2f> mPt;

			perspectiveTransform(cameraPt, mPt, data.transformMat);

			float xc = mPt[4].x;
			float yc = mPt[4].y;

			if(data.getCarObj())
				data.getCarObj()->updateCoor(mPt[0].x, mPt[0].y, mPt[1].x, mPt[1].y, mPt[2].x, mPt[2].y,mPt[3].x, mPt[3].y, xc, yc);
			else{
				Car *carObj = new Car(mPt[0].x, mPt[0].y, mPt[1].x, mPt[1].y, mPt[2].x, mPt[2].y,mPt[3].x, mPt[3].y, xc, yc);
				data.createCarObject(carObj);
			}
		}
	}
}
