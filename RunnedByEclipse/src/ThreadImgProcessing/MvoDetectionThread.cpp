/*
 * MvoDetectionThread.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#include "../ThreadImgProcessing/MvoDetectionThread.h"


void MvoDetectionThread::mvoDetection(Mat &img,DataBundle &data, float time){
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

//			Rect r = boundingRect(cnts[i]);
			RotatedRect rrect = minAreaRect(cnts[i]);
			Point2f vPt[4]; rrect.points( vPt );

			vector<Point2f> cameraPt;
			cameraPt.push_back(Point2f(rrect.center.x,rrect.center.y));
			cameraPt.push_back(Point2f(vPt[0].x,vPt[0].y));
			cameraPt.push_back(Point2f(vPt[1].x,vPt[1].y));
			cameraPt.push_back(Point2f(vPt[2].x,vPt[2].y));
			cameraPt.push_back(Point2f(vPt[3].x,vPt[3].y));

//			cameraPt.push_back(Point2f(r.x+r.width/2,r.y+r.height/2));
//			cameraPt.push_back(Point2f(r.x,r.y));
//			cameraPt.push_back(Point2f(r.x+r.width,r.y));
//			cameraPt.push_back(Point2f(r.x,r.y+r.height));
//			cameraPt.push_back(Point2f(r.x+r.width,r.y+r.height));
			vector<Point2f> mPt;

			perspectiveTransform(cameraPt, mPt, data.getTransformMat());

			float xc = mPt[0].x;
			float yc = mPt[0].y;

			if(!data.getCarData()){
				Car *carObj = new Car();
				data.createCarObject(carObj);
//				data.getCarObj()->updateCoor(mPt[0].x, mPt[0].y, mPt[1].x, mPt[1].y, mPt[2].x, mPt[2].y,mPt[3].x, mPt[3].y, xc, yc);
//				Car *carObj = new Car(mPt[0].x, mPt[0].y, mPt[1].x, mPt[1].y, mPt[2].x, mPt[2].y,mPt[3].x, mPt[3].y, xc, yc);
			}
//			cout<<"size: "<<r.height<<"-"<<r.width<<endl;

			float w = sqrt(pow((vPt[0].x-vPt[1].x),2) + pow((vPt[0].y-vPt[1].y),2));
			float h = sqrt(pow((vPt[1].x-vPt[2].x),2) + pow((vPt[1].y-vPt[2].y),2));
			//			cout<<"size:\n";
//			cout<<"  [0-1]->"<<w<<endl;
//			cout<<"  [1-2]->"<<h<<endl;
			float wb = w>h?w:h;
			cout<<"wheel base: "<<wb<<endl;
			data.getCarData()->setWheelBase(wb);
			data.getCarData()->updateCoor(mPt);
			data.getCarData()->setCenter(xc,yc,time);
			manager->startAllThreads();
		}
	}
}



