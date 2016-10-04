/*
 * img_processing.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: gnouchnam
 */
#include "img_processing.h"

vector<Vec4i> lineDetection(Mat &src, string name);

Mat imgProcessing (Mat &input){

	Mat cleanedInput = Mat(input);
	int newRow = input.rows * 1.0/5;
	//rectangle(input,Point(0,0),Point(input.cols - 1, newRow), Scalar(0,0,0),CV_FILLED);

	Mat hlsImg;
	cvtColor(input,hlsImg,CV_BGR2HLS);

	vector<Mat> hlsChannels(3);
	split(hlsImg,hlsChannels);

	// extract white area
	Mat whiteMask, whiteImg;
	inRange(hlsChannels[1],Scalar(200),Scalar(255),whiteMask);
	bitwise_and(hlsChannels[1],hlsChannels[1],whiteImg,whiteMask);

	vector<Vec4i> whiteLines = lineDetection(whiteImg, "white img");

	// extract yellow area
	Mat yellowMask, yellowImg, satMask;
	inRange(hlsChannels[0],Scalar(10),Scalar(30),yellowMask);
	threshold(hlsChannels[2],satMask,70,255,CV_THRESH_BINARY);
	bitwise_and(hlsChannels[0],satMask,yellowImg,yellowMask);

	threshold(yellowImg,yellowImg,0,255,CV_THRESH_BINARY);
	vector<Vec4i> yellowLines = lineDetection(yellowImg, "yellow img");

	for( size_t i = 0; i < whiteLines.size(); i++ )
	{
	  Vec4i l = whiteLines[i];
	  line( cleanedInput, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, CV_AA);
	}
	for( size_t i = 0; i < yellowLines.size(); i++ )
	{
	  Vec4i l = yellowLines[i];
	  line( cleanedInput, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
	}

	return cleanedInput;
}

vector<Vec4i> lineDetection(Mat &src, string name){
	Mat cannyMorpSrc, morphSrc;
	int morph_size = 4;
	int morph_elem = 2;
	Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

	morphologyEx(src,morphSrc,MORPH_CLOSE, element);
	MorphologicalThinning(morphSrc,cannyMorpSrc);
//	imshow("thin img"+name, cannyMorpSrc);
	Canny(cannyMorpSrc,cannyMorpSrc,50,200,3);

	Mat houghLines = Mat(cannyMorpSrc);
//	vector<Vec2f> lines;
//	HoughLines(cannyMorpSrc, lines, 1, CV_PI/180, 70, 0, 0 );
//

//	for( size_t i = 0; i < lines.size(); i++ )
//	{
//	  float rho = lines[i][0], theta = lines[i][1];
//	  Point pt1, pt2;
//	  double a = cos(theta), b = sin(theta);
//	  double x0 = a*rho, y0 = b*rho;
//	  pt1.x = cvRound(x0 + 1000*(-b));
//	  pt1.y = cvRound(y0 + 1000*(a));
//	  pt2.x = cvRound(x0 - 1000*(-b));
//	  pt2.y = cvRound(y0 - 1000*(a));
//	  line( houghLines, pt1, pt2, Scalar(255,0,0), 3, CV_AA);
//	}

	vector<Vec4i> lines;
	HoughLinesP(cannyMorpSrc, lines, 1, CV_PI/180, 50, 50, 10 );

	return lines;
}

void MorphologicalThinning(Mat &pSrc, Mat &pDst) {
        bool bDone = false;
        int rows = pSrc.rows;
        int cols = pSrc.cols;
        /// pad source
        Mat p_enlarged_src = Mat(rows + 2, cols + 2, CV_32FC1);
        for(int i = 0; i < (rows+2); i++) {
        	p_enlarged_src.at<float>(i, 0) = 0.0f;
        	p_enlarged_src.at<float>(i, cols+1) = 0.0f;
        }
        for(int j = 0; j < (cols+2); j++) {
        	p_enlarged_src.at<float>(0, j) = 0.0f;
        	p_enlarged_src.at<float>(rows+1, j) = 0.0f;
        }
        cout << pSrc.type()<< endl;
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if (pSrc.at<char>(i, j) != 0) {
                        	p_enlarged_src.at<float>(i+1, j+1) = 1.0f;
                        }
                        else
                        	p_enlarged_src.at<float>(i+1, j+1) = 0.0f;
                }
        }

        /// start to thin
        Mat p_thinMat1,p_thinMat2,p_cmp;
        while (bDone != true) {
                /// sub-iteration 1
                ThinSubiteration1(p_enlarged_src, p_thinMat1);
                /// sub-iteration 2
                ThinSubiteration2(p_thinMat1, p_thinMat2);
                /// compare
                compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
                /// check
                int num_non_zero = countNonZero(p_cmp);
                if(num_non_zero == (rows + 2) * (cols + 2)) {
                        bDone = true;
                }
                /// copy
                p_thinMat2.copyTo(p_enlarged_src);

        }
        p_enlarged_src.convertTo(pDst,CV_8UC1,255);
}

void ThinSubiteration1(Mat & pSrc, Mat & pDst) {
    int rows = pSrc.rows;
    int cols = pSrc.cols;
    pSrc.copyTo(pDst);
    for(int i = 1; i < rows-1; i++) {
            for(int j = 1; j < cols-1; j++) {
                    if(pSrc.at<float>(i, j) == 1) {
                            /// get 8 neighbors
                            /// calculate C(p)
                            int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                            int neighbor1 = (int) pSrc.at<float>( i-1, j);
                            int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                            int neighbor3 = (int) pSrc.at<float>( i, j+1);
                            int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                            int neighbor5 = (int) pSrc.at<float>( i+1, j);
                            int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                            int neighbor7 = (int) pSrc.at<float>( i, j-1);
                            int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                                             int(~neighbor3 & ( neighbor4 | neighbor5)) +
                                             int(~neighbor5 & ( neighbor6 | neighbor7)) +
                                             int(~neighbor7 & ( neighbor0 | neighbor1));
                            if(C == 1) {
                                    /// calculate N
                                    int N1 = int(neighbor0 | neighbor1) +
                                                     int(neighbor2 | neighbor3) +
                                                     int(neighbor4 | neighbor5) +
                                                     int(neighbor6 | neighbor7);
                                    int N2 = int(neighbor1 | neighbor2) +
                                                     int(neighbor3 | neighbor4) +
                                                     int(neighbor5 | neighbor6) +
                                                     int(neighbor7 | neighbor0);
                                    int N = min(N1,N2);
                                    if ((N == 2) || (N == 3)) {
                                            /// calculate criteria 3
                                            int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
                                            if(c3 == 0) {
                                                    pDst.at<float>( i, j) = 0.0f;
                                            }
                                    }
                            }
                    }
            }
    }
}


void ThinSubiteration2(Mat & pSrc, Mat & pDst) {
    int rows = pSrc.rows;
    int cols = pSrc.cols;
    pSrc.copyTo( pDst);
    for(int i = 1; i < rows-1; i++) {
            for(int j = 1; j < cols-1; j++) {
                    if (pSrc.at<float>( i, j) == 1) {
                            /// get 8 neighbors
                            /// calculate C(p)
                        int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                        int neighbor1 = (int) pSrc.at<float>( i-1, j);
                        int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                        int neighbor3 = (int) pSrc.at<float>( i, j+1);
                        int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                        int neighbor5 = (int) pSrc.at<float>( i+1, j);
                        int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                        int neighbor7 = (int) pSrc.at<float>( i, j-1);
                            int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                                    int(~neighbor3 & ( neighbor4 | neighbor5)) +
                                    int(~neighbor5 & ( neighbor6 | neighbor7)) +
                                    int(~neighbor7 & ( neighbor0 | neighbor1));
                            if(C == 1) {
                                    /// calculate N
                                    int N1 = int(neighbor0 | neighbor1) +
                                            int(neighbor2 | neighbor3) +
                                            int(neighbor4 | neighbor5) +
                                            int(neighbor6 | neighbor7);
                                    int N2 = int(neighbor1 | neighbor2) +
                                            int(neighbor3 | neighbor4) +
                                            int(neighbor5 | neighbor6) +
                                            int(neighbor7 | neighbor0);
                                    int N = min(N1,N2);
                                    if((N == 2) || (N == 3)) {
                                            int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
                                            if(E == 0) {
                                                    pDst.at<float>(i, j) = 0.0f;
                                            }
                                    }
                            }
                    }
            }
    }
}
