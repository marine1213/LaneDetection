/*
 * LineTest.cpp
 *
 *  Created on: Oct 13, 2016
 *      Author: gnouchnam
 */


#include "Line.h"

int main(){
	Line l = Line(0,-139,139,0);
//	cout << l.getAngle()<<endl;
//	cout << l.getFreeCoeff()<<endl<< l.isOn(4,5) <<endl<<l.distance(0,0)<<endl;
	float p[2];
	Line l2 = Line(0,0,1,0);
//	Line l3 = Line(0,0,9,9);
	l2.cross(l,p);
	cout<<Point2f(p[0],p[1])<<endl;
//	l3.cross(l,p);
//	cout<<Point2f(p[0],p[1])<<endl;
	return 0;
}
