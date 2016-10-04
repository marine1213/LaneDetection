#ifndef _COMMON_H_
#define _COMMON_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

// Type of input (enable for an input only)
#define STATIC_IMAGE				0
#define VIDEO						0
#define CAMERA						1


// Dataset configurations
#define VIDEO_FILE					"../../../Dataset/0811/out2.wmv"
#define DATASET_DIR					"../../../Dataset/RoadSamples/"

// Image size
#define COLS					860
#define ROWS					540



#endif //_COMMON_H_
