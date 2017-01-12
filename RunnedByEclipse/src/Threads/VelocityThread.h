/*
 * DistanceToStopLineThread.h
 *
 *  Created on: Dec 20, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_THREADS_VELOCITYTHREAD_H_
#define SRC_THREADS_VELOCITYTHREAD_H_

#include "../System/SubThread.h"
#include "../Controller/CarCtrl.h"

class VelocityThread : public SubThread{
public:
	VelocityThread(CarCtrl *sourceCtl){
		carCtrl 	= sourceCtl;
		velocity	= 0;
	};

	void startThread(){
		this->exec();
	}

private:
	CarCtrl *carCtrl;
	float	velocity;

	void content(){
		PointSeries *ps = carCtrl->getData()->getCarData()->getBuff50Diff();
		if(ps->size()>2){
			float result = 0;
			int size = (ps->size() < 10)?ps->size():10;

			for (int i = 0; i < size; ++i) {
				result += sqrt(pow(ps->get(i)[0],2) + pow(ps->get(i)[1],2));
			}
			velocity = result/abs(ps->get(0)[2] - ps->get(size - 1)[2]);
//			cout<<"v = "<<velocity<<endl;
		}
	}
};


#endif /* SRC_THREADS_VELOCITYTHREAD_H_ */
