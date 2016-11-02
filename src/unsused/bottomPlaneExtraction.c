//
//void bottomPlaneDetection(Mat &input);
//vector<Point> bottomExtraction(Mat &input, int x, int y);
//void bottomPlaneDetection(Mat &input){
//
//	vector<vector<Point> > cnts;
//	vector<Vec4i> hier;
//
//	/// Find contours
//	findContours(input, cnts, hier, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//
//	/// Draw contours
//	Mat drClone = Mat::zeros(input.size(), CV_8UC1);
//	Mat drClone2 = Mat::zeros(input.size(), CV_8UC1);
//
//	for (size_t i = 0; i < cnts.size(); i++) {
//		if(contourArea(cnts[i]) > 50){
//			drawContours(drClone, cnts, i, Scalar(255), 1, 4, hier, 0, Point());
//			Rect rect = boundingRect(cnts[i]);
//			Mat cropRoi = drClone(rect);
//
//			vector<Point> bt = bottomExtraction(cropRoi, rect.x,rect.y);
//			// draw bottom line
//			if(bt.size() >0){
//				for (int i = 0; i < bt.size() - 1; ++i) {
//					line(drClone2,Point(bt[i].x+rect.x,bt[i].y+rect.y),Point(bt[i+1].x + rect.x,bt[i+1].y+rect.y),Scalar(255),3,8);
//				}
//			}
//		}
//	}
//	imshow("cnt mov", drClone2);
//	imshow("cnt mov cnt", drClone);
//	waitKey(10);
//}
//
//vector<Point> bottomExtraction(Mat &input, int x, int y){
//	vector<Point> bottom;
//	for (int col = 0; col < input.cols; ++col) {
//		for (int row = input.rows - 1; row >0 ; --row) {
//			if(input.at<uchar>(row,col) == 255){
//				bottom.push_back(Point(col,row));
//				break;
//			}
//		}
//	}
//	vector<Point> approxBt;
//	if(bottom.size() >0 ){
//		double eps = bottom.size() / 10.0;
//		approxPolyDP(bottom,approxBt,eps,0);
//	}
//	return approxBt;
//}
