

//	vector<Vec2f> lines;
//	HoughLines(cannyMorpSrc, lines, 1, CV_PI / 180, 70, 0, 0);

//	for (size_t i = 0; i < whiteLines.size(); i++) {
//		float rho = whiteLines[i][0], theta = whiteLines[i][1];
//		Point pt1, pt2;
//		double a = cos(theta), b = sin(theta);
//		double x0 = a * rho, y0 = b * rho;
//		pt1.x = cvRound(x0 + 1000 * (-b));
//		pt1.y = cvRound(y0 + 1000 * (a));
//		pt2.x = cvRound(x0 - 1000 * (-b));
//		pt2.y = cvRound(y0 - 1000 * (a));
//		line(input, pt1, pt2, Scalar(255, 0, 0), 3, CV_AA);
//	}
//
//	for (size_t i = 0; i < yellowLines.size(); i++) {
//		float rho = yellowLines[i][0], theta = yellowLines[i][1];
//		Point pt1, pt2;
//		double a = cos(theta), b = sin(theta);
//		double x0 = a * rho, y0 = b * rho;
//		pt1.x = cvRound(x0 + 1000 * (-b));
//		pt1.y = cvRound(y0 + 1000 * (a));
//		pt2.x = cvRound(x0 - 1000 * (-b));
//		pt2.y = cvRound(y0 - 1000 * (a));
//		line(input, pt1, pt2, Scalar(0, 255, 0), 3, CV_AA);
//	}
