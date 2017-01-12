/*
 * Intersection.h
 *
 *  Created on: Nov 8, 2016
 *      Author: gnouchnam
 */

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "IntersectionBorder.h"
#include "LinesConnector.h"
#include "opencv2/highgui.hpp"

class Intersection{
private:
	int id;
	IntersectionBorder *borders[4];
	float limitArea[4];

	void setBorders(Line *left, Line *top, Line *right, Line *bottom){
		borders[BORDER_LEFT] 	= new IntersectionBorder(left,BORDER_LEFT);
		borders[BORDER_TOP]		= new IntersectionBorder(top,BORDER_TOP);
		borders[BORDER_RIGHT] 	= new IntersectionBorder(right,BORDER_RIGHT);
		borders[BORDER_BOTTOM] 	= new IntersectionBorder(bottom,BORDER_BOTTOM);
	}
public:

	Intersection(Line *left, Line *top, Line *right, Line *bottom){
		setBorders(left, top, right, bottom);
	}

	Intersection(int width, int height){
		Line *left		= new Line(0,0,0,height);
		Line *top		= new Line(0,0,width,0);
		Line *right		= new Line(width,0,width,height);
		Line *bottom	= new Line(0,height,width,height);
		setBorders(left, top, right, bottom);
	}

	~Intersection(){
#if SHOW_CLASSES_STATE
		cout<<"Intersection Destructor"<<endl;
#endif
		for (int i = 0; i < 4; ++i) {
#if SHOW_CLASSES_STATE
			cout << "deleting border " << i << endl;
#endif
			delete borders[i];
		}
	}

	LinesConnector* createPath(int departId, int arrivedId){
		return borders[departId]->pathTo(borders[arrivedId]);
	}

	//===================== support control part ===========================

	void setupCenterArea(){
		for (int i = 0; i < 4; ++i)
			limitArea[i] = borders[i]->getCenterIntersectionAreaEdge();
	}

	bool withinCeterArea(float x, float y){
		if(cv::waitKey(10)==' ')
			y = 0 + y;
		bool xCondition = (x >= limitArea[BORDER_LEFT] - 1 && x <= limitArea[BORDER_RIGHT] + 1);
		bool yCondition = (y >= limitArea[BORDER_TOP] - 1&& y <= limitArea[BORDER_BOTTOM] + 1);
		return xCondition && yCondition;
	}

	//========================== setup path ================================

	/**
	 * Return id of nearest border of a point
	 */
	int findDenpendedBorder(float x, float y){
		int idMinDistance = 0;
		float minDistance = borders[0]->getBorderLine()->distance(x,y);
		for (int i = 1; i < 4; ++i) {
			float d = borders[i]->getBorderLine()->distance(x,y);
			if(d < minDistance){
				idMinDistance = i;
				minDistance = d;
			}
		}
		return idMinDistance;
	}

	/**
	 * Return id of nearest border of a bounding rectangle
	 */
	int findDenpendedBorder(Rect r){
		int idMinDistance = 0;
		float minDistance = INT32_MAX;
		int vertices[8] = {r.x,r.y,r.x+r.width,r.y,r.x,r.y+r.height,r.x+r.width,r.y+r.height};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; j+=2) {
				float d = borders[i]->getBorderLine()->distance(vertices[j], vertices[j+1]);
				if (d < minDistance) {
					idMinDistance = i;
					minDistance = d;
				}
			}
		}
		return idMinDistance;
	}

	/**
	 * recognize stop line and right line of each border
	 */
	void recognizeStopLine(vector<Line> &lines, int borderId, Rect *limRect = NULL){
		vector<Line> stopLines;
		vector<Line> rightLines;
		for (size_t i = 0; i < lines.size(); ++i) {
			float deltaAngle = abs(borders[borderId]->getBorderLine()->angle(lines[i]));
			if(deltaAngle < 10)
				stopLines.push_back(lines[i]);
			else if(deltaAngle>80)
				rightLines.push_back(lines[i]);
			else
				assert("Invalid delta angle with intersection border" == 0);
		}
		Lane *stopLane = new Lane(new Line(stopLines[0]),new Line(stopLines[1]),WHITE_LANE);
		Lane *rightLane = new Lane(new Line(rightLines[0]), new Line(rightLines[1]),WHITE_LANE);
		stopLane->setType(LANE_STOP);
		rightLane->setType(LANE_RIGHT);

		addLane(stopLane,borderId, limRect);
		addLane(rightLane,borderId, limRect);
	}

	/**
	 * add lanes to intersection based on border and lane type
	 */
	void addLane(Lane *lane,int idOfBorder, Rect *limRect = NULL){
		switch (lane->getType()) {
			case LANE_MID:
				if(lane->getColor() == YELLOW_LANE)
					borders[idOfBorder]->setYellowLane(lane,limRect);
				else
					assert("lane color is wrong" == 0);
				break;
			case LANE_LEFT:
				borders[idOfBorder]->setLeftLane(lane,limRect);
				break;
			case LANE_RIGHT:
				borders[idOfBorder]->setRightLane(lane,limRect);
				break;
			case LANE_STOP:
				borders[idOfBorder]->setStopLane(lane,limRect);
				break;
			default:
				assert("lane type is wrong" == 0);
				break;
		}
	}

	int getId(){
		return id;
	}

	/**
	 * Return borders data
	 */
	IntersectionBorder** getBorders() {
		return borders;
	}

	/**
	 * Interpolate imagine lines to control the car
	 */
	void interpolateLines(Mat *img){
		for (int i = 0; i < 4; ++i) {
			if(borders[i]->isLaneDataValid())
				borders[i]->interpolateLines(img);
		}
	}
};


#endif /* INTERSECTION_H_ */
