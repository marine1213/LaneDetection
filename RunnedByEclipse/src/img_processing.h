#ifndef _IMG_PROCESSING_
#define _IMG_PROCESSING_

#include "common.h"
#include "Model/Line.h"
#include "Model/Lane.h"
#include "Model/Car.h"
#include "Model/Intersection.h"
#include "Model/DataStructure.h"
#include "my_tools.h"


#define MAP_WIDTH		650
#define MAP_HEIGHT		650


// Function headers
void perspectiveTransform(Mat &input, Mat &output);
void laneProcessing(Mat &input, int imgId,Data &data);
void mvoFilter(Mat &input, Mat &output,Data &data);
void setupPPMap(Mat &input,Mat &transformMat, Mat & ppmap);



#endif // !_IMG_PROCESSING_
