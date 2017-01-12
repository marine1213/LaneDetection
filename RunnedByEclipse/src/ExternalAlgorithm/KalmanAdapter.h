/*
 * testKalman.h
 *
 *  Created on: Dec 29, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_TESTKALMAN_H_
#define SRC_TESTKALMAN_H_

#include "../common.h"
#include "opencv2/video/tracking.hpp"


int testKalman();

class KalmanAdapter {
private:
	KalmanFilter KF;
	Mat_<float> measurement;
	bool		initialized;
public:
	KalmanAdapter(){
		KF = KalmanFilter(4,2,0);
		initialized = false;
	}

	void init(float x, float y){
		KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0,   0, 1, 0, 1,   0, 0, 1, 0,   0, 0, 0, 1);
		measurement = (Mat_<float>(2,1)<< x, y);

		KF.statePost.at<float>(0) = x;
		KF.statePost.at<float>(1) = y;
		KF.statePost.at<float>(2) = 0;
		KF.statePost.at<float>(3) = 0;
		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
		setIdentity(KF.measurementNoiseCov, Scalar::all(3));
		setIdentity(KF.errorCovPost, Scalar::all(.1));
		initialized = true;
	}

	Point2f update(float x, float y){
		if(!initialized)
			init(x,y);
		// First predict, to update the internal statePre variable
		Mat prediction = KF.predict();
		Point2f predictPt(prediction.at<float>(0), prediction.at<float>(1));

		measurement(0) = x;
		measurement(1) = y;

		// The update phase
		Mat estimated = KF.correct(measurement);

		Point2f statePt(estimated.at<float>(0), estimated.at<float>(1));
		return statePt;
	}
};

#endif /* SRC_TESTKALMAN_H_ */
