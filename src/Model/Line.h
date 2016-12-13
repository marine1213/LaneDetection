/*
 * Line.h
 *
 *  Created on: Oct 13, 2016
 *      Author: gnouchnam
 */

#ifndef LINE_H_
#define LINE_H_
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;


class Line {
private:
	float point[2], point2[2];
	float cosAngleOx;
	float angleToOx;
	float freeCoeff;
	Mat vtcp, vtpt;
	float vtcpfl[2], vtptfl[2];

	float calcAngleToOx() {
		float coMul = vtcp.at<float>(0, 0);
		float coLen = sqrt(sum(vtcp.mul(vtcp))[0]);
		float coeff = (vtcp.at<float>(1, 0)<0)?-1:1;
		return coeff*(coMul / coLen);
	}

	void calcFormula() {
		freeCoeff = -(vtptfl[0]*point[0] + vtptfl[1]*point[1]);
	}

	Mat vect(float dx, float dy){
		float vt[2] = { dx, dy };
		return Mat(2, 1, CV_32F, vt);
	}

	void setPoint(float x1, float y1, float x2, float y2){
		if(x1*x1+y1*y1 < x2*x2 + y2*y2){
			point[0] = x1;
			point[1] = y1;
			point2[0] = x2;
			point2[1] = y2;
		}else{
			point[0] = x2;
			point[1] = y2;
			point2[0] = x1;
			point2[1] = y1;
		}
	}

public:

	Line(float x1, float y1, float x2, float y2){
		setPoint(x1,y1,x2,y2);

		vtcpfl[0] = x2 - x1;
		vtcpfl[1] = y2 - y1;
		vtptfl[0] = y2 - y1;
		vtptfl[1] = x1 - x2;
		vtcp = Mat(2, 1, CV_32F, vtcpfl);
		vtpt = Mat(2, 1, CV_32F, vtptfl);
		assert(countNonZero(vtcp)!=0 && "directional vector should not be zero");
		cosAngleOx 	= calcAngleToOx();
		angleToOx	= acos(cosAngleOx) * 180 / M_PI;
		calcFormula();
	}

	Line* findMidLine(Line &other){
		return findMidLine(other, 0.5);
	}
	Line* findMidLine(Line &other, float ratio){
		float ptDt1[2],ptDt2[2],ptDt[2];

		Line Ox = Line(0,0,1,0);
		cross(Ox,ptDt1);
		other.cross(Ox,ptDt2);

		ptDt[0] = ptDt1[0]*ratio + (1 - ratio)*ptDt2[0];
		ptDt[1] = ptDt1[1]*ratio + (1 - ratio)*ptDt2[1];

		float rad= (angleToOx*ratio + (1 - ratio)*other.angleToOx)*M_PI/180; // (/2)(PI/180)
		float x1 = ptDt[0] - 1000*cos(rad);
		float y1 = ptDt[1] - 1000*sin(rad);
		float x2 = ptDt[0] + 1000*cos(rad);
		float y2 = ptDt[1] + 1000*sin(rad);
		return new Line(x1,y1,x2,y2);
	}

	void cross(Line &other, float* p){
		float D = vtptfl[0] * other.vtptfl[1] - vtptfl[1] * other.vtptfl[0]; // D = a1*b2 - a2*b1
		if(D==0){
			p[0] = 0;
			p[1] = 0;
			return;
			//assert(D!=0 && "Two lines are parallel or the same line");
		}
		float Dx = (-freeCoeff)* other.vtptfl[1] - (-other.freeCoeff) * vtptfl[1]; // Dx = c1*b2 - c2*b1
		float Dy = vtptfl[0]* (-other.freeCoeff) - other.vtptfl[0]* (-freeCoeff);		 // Dy = a1*c2 - a2*c1
		p[0] = Dx/D;
		p[1] = Dy/D;
	}

	float distance(float x, float y){
		return abs(isOn(x,y))/sqrt(pow(vtptfl[0],2)+pow(vtptfl[1],2));
	}

	float isOn(float x,float y){
		return (vtptfl[0]*x + vtptfl[1]*y + freeCoeff);
	}

	bool isOn(Line line){
		if(abs(line.angleToOx - angleToOx) < 2)
			return (distance(line.point[0], line.point[1]) < 3);
		return false;
	}

	void setLimitedArea(float left, float above,float right, float under) {
		int u = under - 1, r = right - 1;
		Line boundaries[4] = {
				Line(left, above, left+1, above),
				Line(left, above, left, above+1),
				Line(r, above, r,u),Line(left, u, r, u) };
		float pts[2];
		for (int i = 0,id= 0; i < 4; ++i) {
			cross(boundaries[i],pts+id);

			if((pts+id)[0] >= left && (pts+id)[0] <= r && (pts+id)[1] >= above && (pts+id)[1] <= u){
				id += 2;
				if(id == 4)
					break;
			}
		}
		setPoint(pts[0],pts[1],pts[2],pts[3]);
	}

	float getAngle() {return angleToOx;}
	float getCosAngle() {return cosAngleOx;}
	float getFreeCoeff() {return freeCoeff;}

	float* getVTCP(){return vtcpfl;}
	float* getVTPT(){return vtptfl;}

	float* getPoint(){	return point;}
	float* getPoint2(){	return point2;}

};

#endif /* LINE_H_ */
