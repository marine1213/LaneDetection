#ifndef _IMG_PROCESSING_
#define _IMG_PROCESSING_

#include "common.h"
#include "Model/Line.h"
#include "Model/Lane.h"
#include "Model/Car.h"
#include "Model/DataStructure.h"
#include "my_tools.h"


#define ELM_OPEN	getStructuringElement(1, Size(1, 1));


// Function headers
void perspectiveTransform(Mat &input, Mat &output);
void laneProcessing(Mat &input, int imgId,Data &data);
void mvoFilter(Mat &input, Mat &output,Data &data);



#endif // !_IMG_PROCESSING_
