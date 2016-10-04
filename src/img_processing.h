#ifndef _IMG_PROCESSING_
#define _IMG_PROCESSING_

#include "common.h"
#include "my_tools.h"

// Function headers
Mat	imgProcessing(Mat &input);

/*
 * Source:
 * http://answers.opencv.org/question/3207/what-is-a-good-thinning-algorithm-for-getting-the-skeleton-of-characters-for-ocr/
 * */
void ThinSubiteration1(Mat & pSrc, Mat & pDst);
void ThinSubiteration2(Mat & pSrc, Mat & pDst);
void MorphologicalThinning(Mat &pSrc, Mat &pDst);



#endif // !_IMG_PROCESSING_
