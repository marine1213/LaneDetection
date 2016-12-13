/*
 * img_processing.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: gnouchnam
 */
#include "img_processing.h"
#include "draw_helper.h"

void laneDetection(Mat &src, LaneType laneType, vector<Lane*> &vLane);
void get4Vertices(vector<Lane*> vLane, Size s,Point2f* output);
void interpolateLane(vector<Lane*> &vLane, vector<Line*> &output);

void select_points(int event, int x, int y, int flags, void* userdata);


void perspectiveTransform(Mat &input, Mat &output){
	input.copyTo(output);
}

void select_points(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN){//&& flags == EVENT_FLAG_CTRLKEY
		cout << "Left button clicked pos x: " << x << ", pos y: " << y << endl;
		Point2i *p = (Point2i *)userdata;
		p->x = x;
		p->y = y;
	}
}

void laneProcessing(Mat &input, int imgId,Data &data) {

	if (imgId) {
		//-------Delete 1/3 upper area of image to remove sky-----
		//	int newRow = input.rows * 1.0 / 3;
		//	rectangle(cleanedInput, Point(0, 0), Point(input.cols - 1, newRow),
		//			Scalar(0, 0, 0), CV_FILLED);

		//---------------------color filter-----------------------
		Mat hlsImg;
		cvtColor(input, hlsImg, CV_BGR2HLS);

		Mat yellowMask, whiteMask;		// blackMaskHls;
		inRange(hlsImg, Scalar(10, 0, 100), Scalar(30, 255, 255), yellowMask);
		inRange(hlsImg, Scalar(0, 200, 0), Scalar(255, 255, 255), whiteMask);
//		inRange(hlsImg, Scalar(0, 65, 0), Scalar(255, 145, 255), blackMaskHls);  // to get black area

		//----------------extract lanes----------------------

		laneDetection(whiteMask, WHITE_LANE, data.lanes);
		//cout<<"Num of white lanes:"<<whiteLanes.size()<<endl;
		vector<Lane*> yellowLanes;
		laneDetection(yellowMask, YELLOW_LANE, yellowLanes);
		//cout<<"Num of yellow lanes:"<<yellowLanes.size()<<endl;

		//-----replace 2 lanes are on the same position-------
		for (int i = 0; i < yellowLanes.size(); ++i) {
			bool isReplaced = false;
			for (int j = 0; j < data.lanes.size(); ++j) {
				if(yellowLanes[i]->isOn(*(data.lanes[j]))){
					delete (data.lanes[j]);
					data.lanes[j] = yellowLanes[i];
					isReplaced = true;
				}
			}
			if(!isReplaced)
				data.lanes.push_back(yellowLanes[i]);
		}
		//---------interpolate centerLine-----------------------
		interpolateLane(data.lanes,data.interpolatedLines);
	}
}

void laneDetection(Mat &src, LaneType laneType, vector<Lane*> &vLane) {
	// prepare for close operator
	Mat canMorp, morphSrc;
	Mat element = getStructuringElement(2, Size(9, 9), Point(4, 4));

	morphologyEx(src, morphSrc, MORPH_CLOSE, element); // close to fill the gaps in thick lane
	Canny(morphSrc, canMorp, 50, 200, 3);		// get border of the lane

	// prepare for contour operator
	vector<vector<Point> > cnts;
	vector<Vec4i> hier;
	/// Find contours
	findContours(canMorp, cnts, hier, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	/// Draw contours
	Mat empty = Mat::zeros(canMorp.size(), CV_8UC1);
	Mat draw = Mat::zeros(canMorp.size(), CV_8UC3);

	vector<Line> vLine;			// Including all lines, to remove existed line

	for (size_t i = 0; i < cnts.size(); i++) {
		Mat drClone = empty.clone();
		drawContours(drClone, cnts, i, Scalar(1), 1, 4, hier, 0, Point());
		//drawContours(draw, cnts, i, Scalar(255, 0, 0), 1, 4, hier, 0, Point());

		// find Hough lines
		vector<Vec4i> lines;
		vector<Line> lineInOneCnt;
		HoughLinesP(drClone, lines, 1, CV_PI / 180, 50, 50, 10);
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			Line tempLine = Line(l[0], l[1], l[2], l[3]);

			bool isNew = true;
			for (int i = 0; i < vLine.size(); ++i) {
				if (vLine[i].isOn(tempLine)) {
					isNew = false;
					break;
				}
			}
			if (isNew) {
				vLine.push_back(tempLine);
				lineInOneCnt.push_back((tempLine));
				line(draw, Point(l[0], l[1]), Point(l[2], l[3]),Scalar(0, 255, 0), 1, CV_AA);
			}
//			ostringstream str;
//			str<<"Contours drawing "<<(laneType==WHITE_LANE?"White lane":"Yellow lane");
//			imshow(str.str(), draw);
		}

		if(lineInOneCnt.size() == 2){
			Lane *lane = new Lane(lineInOneCnt[0],lineInOneCnt[1],laneType);
			vLane.push_back(lane);
		}
	}
}
void get4Vertices(vector<Lane*> vLane,Size s, Point2f* output){
	vector<float> distTo0;
	vector<float> dv;
	// rearrange the order of lane -> has problem
	for (int i = 0; i < vLane.size(); ++i) {
		float d = vLane[i]->getAngle();
		int idToIns = 0;
		for (int j = 0; j < distTo0.size(); ++j) {
			if(d<distTo0[j])
				idToIns = j;
		}
		distTo0.insert(distTo0.begin() + idToIns,i);
		dv.insert(dv.begin() + idToIns,d);
	}

	Lane *leftLane = vLane[distTo0.back()];
	Lane *rightLane = vLane[distTo0[0]];

	leftLane->getLeft()->setLimitedArea(0,0,s.width,s.height);
	rightLane->getRight()->setLimitedArea(0,0,s.width,s.height);
	float crossedPt[2];
	leftLane->getLeft()->cross(*(leftLane->getRight()),crossedPt);
	if(crossedPt[0] >= 0 && crossedPt[0] < s.width && crossedPt[1] >= 0 && crossedPt[1] <= s.height){
		// crossed point is in desired area
		leftLane->getLeft()->setLimitedArea(0,crossedPt[1]+10,s.width,s.height);
		rightLane->getRight()->setLimitedArea(0,crossedPt[1]+10,s.width,s.height);
	}
	output[0] = Point2f(leftLane->getLeft()->getPoint()[0],leftLane->getLeft()->getPoint()[1]);
	output[1] = Point2f(rightLane->getRight()->getPoint()[0],rightLane->getRight()->getPoint()[1]);
	output[2] = Point2f(rightLane->getRight()->getPoint2()[0],rightLane->getRight()->getPoint2()[1]);
	output[3] = Point2f(leftLane->getLeft()->getPoint2()[0],leftLane->getLeft()->getPoint2()[1]);

//	Mat draw = Mat::zeros(s, CV_8UC3);
//	drawLine(*(leftLane->getLeft()),draw);
//	drawLine(*(rightLane->getRight()),draw);
//	imshow("road detected", draw);
	//==================== main code to use =============================
//	if(whiteLanes.size()){
//				Point2f inptsPerspective[4];
//				get4Vertices(whiteLanes,Size(input.cols,input.rows), inptsPerspective);
//				Point2f outptsPerspective[4] = {
//						Point2f(0,0),Point2f(0,input.cols-1),
//						Point2f(input.rows-1,input.cols-1),Point2f(input.rows-1,0)
//				};
//				Mat persMat = getPerspectiveTransform(inptsPerspective,outptsPerspective);
//				Mat persOut = Mat::zeros(input.size(), CV_8UC3);
//				warpPerspective(input,persOut,persMat,persOut.size());
//
//				imshow("Perspective",persOut);
//			}
}

void interpolateLane(vector<Lane*> &vLane, vector<Line*> &output){
	if (vLane.size() > 1) {
		vector<float> angleList;
		vector<float> idList;
		// rearrange the order of lane
		for (int i = 0; i < vLane.size(); ++i) {
			float angle = vLane[i]->getAngle();
			int idToIns = 0;
			for (int j = 0; j < angleList.size(); ++j)
				if(angle < angleList[j])
					idToIns = j;
			angleList.insert(angleList.begin() + idToIns,angle);
			idList.insert(idList.begin() + idToIns,i);
		}
		// find midline of each lane
		for (int i = 0; i < idList.size() - 1; ++i) {
			Line *cL1 = vLane[idList[i]]->getCenter();
			Line *cL2 = vLane[idList[i + 1]]->getCenter();
			Line *vCter = cL1->findMidLine(*(cL2),1.0/2);
			output.push_back(vCter);
			Line *vLine = cL1->findMidLine(*(cL2),3.0/8);
			output.push_back(vLine);
			Line *vLine2 = cL1->findMidLine(*(cL2),5.0/8);
			output.push_back(vLine2);
		}
	}
}

void mvoDetection(Mat &img,Data &outputData);

Mat elm = getStructuringElement(CV_SHAPE_ELLIPSE, Size(5, 5));

void mvoFilter(Mat &input, Mat &output,Data &data){
	Mat grayInput;
	cvtColor(input, grayInput, COLOR_BGR2GRAY);
	data.pMOG2->apply(grayInput, output,0.00);
	morphologyEx(output, output, MORPH_CLOSE, elm);
	imshow("mov obj thres close",output);
	mvoDetection(output,data);
}

void mvoDetection(Mat &img,Data &data){
	vector<vector<Point> > cnts;
	vector<Vec4i> hier;
	/// Find contours
	findContours(img, cnts, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	/// Draw contours
	Mat draw = Mat::zeros(img.size(), CV_8UC3);

	//vector<Line> vLine;			// Including all lines, to remove existed line

	for (size_t i = 0; i < cnts.size(); i++) {
		float area = contourArea(cnts[i]);
		vector<int> vPt;
		if(area>100){
			drawContours(draw, cnts, i, Scalar(0,255,255), 1, 4, hier, 0, Point());
			RotatedRect rrect = minAreaRect(cnts[i]);
			Point2f vPt[4]; rrect.points( vPt );

			float xc = rrect.center.x;
			float yc = rrect.center.y;
			if(data.car)
				data.car->updateCoor(vPt[0].x, vPt[0].y, vPt[1].x, vPt[1].y, vPt[2].x, vPt[2].y,vPt[3].x, vPt[3].y, xc, yc);
			else
				data.car = new Car(vPt[0].x, vPt[0].y, vPt[1].x, vPt[1].y, vPt[2].x, vPt[2].y,vPt[3].x, vPt[3].y, xc, yc);
		}
	}
	draw.copyTo(data.tempMat);
}
