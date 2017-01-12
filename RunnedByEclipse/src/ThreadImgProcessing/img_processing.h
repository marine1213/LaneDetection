#ifndef _IMG_PROCESSING_
#define _IMG_PROCESSING_

#include "../common.h"
#include "../my_tools.h"
#include "../Model/DataBundle.h"


#define MAP_WIDTH		650
#define MAP_HEIGHT		650


// Function headers
void perspectiveTransform(Mat &input, Mat &output);
void laneProcessing(Mat &input, int imgId,DataBundle &data);
void setupPPMap(Mat &input,Mat &transformMat, Mat & ppmap);



#endif // !_IMG_PROCESSING_
