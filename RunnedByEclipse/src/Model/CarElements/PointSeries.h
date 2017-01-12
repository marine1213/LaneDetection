/*
 * SeriesData.h
 *
 *  Created on: Dec 8, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_MODEL_POINTSERIES_H_
#define SRC_MODEL_POINTSERIES_H_

#include "stdlib.h"

class PointSeries{
private:
	float **ptsData;
	int		queueSize, actualSize;
	int		currentId;

public:
	PointSeries(int size){
		queueSize = size;
		currentId = 0;
		actualSize = 0;

		ptsData = (float**)malloc(size*sizeof(float*));
		for (int i = 0; i < size; ++i)
			ptsData[i] = (float*)malloc(3*sizeof(float));
	}

	~PointSeries(){
		for (int i = 0; i < queueSize; ++i) {
			free(ptsData[i]);
		}
		free(ptsData);
	}

	void add(float x, float y, float timestamp){
		ptsData[currentId][0] = x;
		ptsData[currentId][1] = y;
		ptsData[currentId][2] = timestamp;
		currentId = normalizeId(currentId+1);
		actualSize += (actualSize >= queueSize)?0:1;
//		if(actualSize > 100){
//			cout<<actualSize<<endl;
//		}
	}

	void clear(){
		actualSize = 0;
		ptsData[currentId][0] = 0;
		ptsData[currentId][1] = 0;
		ptsData[currentId][2] = 0;
	}

	float* get(int id){
		if(id<actualSize)
			return ptsData[normalizeId(currentId - id)];
		else
			return NULL;
	}

	int size(){
		return actualSize;
	}

private:
	int normalizeId(int id){
		return (id + queueSize) % queueSize;
	}
};



#endif /* SRC_MODEL_POINTSERIES_H_ */
