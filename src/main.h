#ifndef _MAIN_H_
#define _MAIN_H_

#include "common.h"
#include "Model/DataStructure.h"
#include "Controller/CarCtrl.h"
#include "img_processing.h"
#include "main_imgProcessingLib.h"

void main_imgProcessing(int argc, char** args,Data &data);



void main_imgProcessing(int argc, char** args,Data &data){
	// assertions
	assert((VIDEO + CAMERA + STATIC_IMAGE) == 1 && "Only one type of input should be enable at a time");
	init(argc,args);

	int frCount = 0;
	float fps = 0;
	bool isFirsFrm = true;

	char c = 'a', d = 1;
	Mat img;
	clock_t t = clock();
	while(c != 27){
#if STATIC_IMAGE
		if(!(ent = readdir (dir)))
			break;
		img = readImage(ent->d_name);
#if SHOW_INPUT
		if(!img.empty()) // check if file is directory or not image
			imshow(ent->d_name, img);
#endif //SHOW_INPUT
#elif VIDEO || CAMERA
		img = readImage(capture);
		frCount++;
		clock_t delta = clock() - t;
		float sec = (float)delta/CLOCKS_PER_SEC;
		if( sec > 1){
			fps = frCount/sec;
			t = clock();
			frCount = 0;
		}
#if SHOW_INPUT
		if(img.empty())
			break;
		imshow("Preview", img);
#endif //SHOW_INPUT
#endif
		if(img.empty())
			continue;
		Mat mvoOutput,map;
		if(isFirsFrm){
			perspectiveTransform(img,map);
			laneProcessing(map, d, data);
			isFirsFrm = false;
		}

		mvoFilter(img, mvoOutput, data);
		Mat draw = Mat::zeros(img.size(), CV_8UC3);

		data.showInterpolatedLineData(data.tempMat);
		data.showLaneData(data.tempMat);
		data.showCar(data.tempMat);

		data.overLaneAssessment();
		data.clearCarData();
//		data.clear();


		ostringstream ss;
		ss << fps;
		putText(data.tempMat, ss.str(), Point(30, 30), FONT_HERSHEY_COMPLEX, 1,
				Scalar(255, 255, 255), 1, CV_AA);
		imshow("OUTPUT", data.tempMat);

		//d++;

		c = waitKey(WAIT_TIME);
	}
	onExit();
}

#endif // !_MAIN_H_
