//
///*
// * Source:
// * http://answers.opencv.org/question/3207/what-is-a-good-thinning-algorithm-for-getting-the-skeleton-of-characters-for-ocr/
// * */
//void ThinSubiteration1(Mat & pSrc, Mat & pDst);
//void ThinSubiteration2(Mat & pSrc, Mat & pDst);
//void MorphologicalThinning(Mat &pSrc, Mat &pDst);
//
//
//
//void MorphologicalThinning(Mat &pSrc, Mat &pDst) {
//	bool bDone = false;
//	int rows = pSrc.rows;
//	int cols = pSrc.cols;
//	/// pad source
//	Mat p_enlarged_src = pSrc;
//
//	/// start to thin
//	Mat p_thinMat1, p_thinMat2, p_cmp;
//	while (bDone != true) {
//		/// sub-iteration 1
//		ThinSubiteration1(p_enlarged_src, p_thinMat1);
//		/// sub-iteration 2
//		ThinSubiteration2(p_thinMat1, p_thinMat2);
//		/// compare
//		compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
//		/// check
//		int num_non_zero = countNonZero(p_cmp);
//		if (num_non_zero == (rows) * (cols)) {
//			bDone = true;
//		}
//		/// copy
//		p_thinMat2.copyTo(p_enlarged_src);
//
//	}
//	pDst = p_enlarged_src;
//}
//
//void ThinSubiteration1(Mat & pSrc, Mat & pDst) {
//	int rows = pSrc.rows;
//	int cols = pSrc.cols;
//	pSrc.copyTo(pDst);
//	for (int i = 1; i < rows - 1; i++) {
//		for (int j = 1; j < cols - 1; j++) {
//			if (pSrc.at<char>(i, j)) {
//				/// get 8 neighbors
//				/// calculate C(p)
//				int neighbor0 = (int) pSrc.at<char>(i - 1, j - 1) != 0;
//				int neighbor1 = (int) pSrc.at<char>(i - 1, j) != 0;
//				int neighbor2 = (int) pSrc.at<char>(i - 1, j + 1) != 0;
//				int neighbor3 = (int) pSrc.at<char>(i, j + 1) != 0;
//				int neighbor4 = (int) pSrc.at<char>(i + 1, j + 1) != 0;
//				int neighbor5 = (int) pSrc.at<char>(i + 1, j) != 0;
//				int neighbor6 = (int) pSrc.at<char>(i + 1, j - 1) != 0;
//				int neighbor7 = (int) pSrc.at<char>(i, j - 1) != 0;
//				int C = int(~neighbor1 & (neighbor2 | neighbor3))
//						+ int(~neighbor3 & (neighbor4 | neighbor5))
//						+ int(~neighbor5 & (neighbor6 | neighbor7))
//						+ int(~neighbor7 & (neighbor0 | neighbor1));
//				if (C == 1) {
//					/// calculate N
//					int N1 = int(neighbor0 | neighbor1)
//							+ int(neighbor2 | neighbor3)
//							+ int(neighbor4 | neighbor5)
//							+ int(neighbor6 | neighbor7);
//					int N2 = int(neighbor1 | neighbor2)
//							+ int(neighbor3 | neighbor4)
//							+ int(neighbor5 | neighbor6)
//							+ int(neighbor7 | neighbor0);
//					int N = min(N1, N2);
//					if ((N == 2) || (N == 3)) {
//						/// calculate criteria 3
//						int c3 = (neighbor1 | neighbor2 | ~neighbor4)
//								& neighbor3;
//						if (c3 == 0) {
//							pDst.at<char>(i, j) = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void ThinSubiteration2(Mat & pSrc, Mat & pDst) {
//	int rows = pSrc.rows;
//	int cols = pSrc.cols;
//	pSrc.copyTo(pDst);
//	for (int i = 1; i < rows - 1; i++) {
//		for (int j = 1; j < cols - 1; j++) {
//			if (pSrc.at<char>(i, j)) {
//				/// get 8 neighbors
//				/// calculate C(p)
//				int neighbor0 = (int) pSrc.at<char>(i - 1, j - 1) != 0;
//				int neighbor1 = (int) pSrc.at<char>(i - 1, j) != 0;
//				int neighbor2 = (int) pSrc.at<char>(i - 1, j + 1) != 0;
//				int neighbor3 = (int) pSrc.at<char>(i, j + 1) != 0;
//				int neighbor4 = (int) pSrc.at<char>(i + 1, j + 1) != 0;
//				int neighbor5 = (int) pSrc.at<char>(i + 1, j) != 0;
//				int neighbor6 = (int) pSrc.at<char>(i + 1, j - 1) != 0;
//				int neighbor7 = (int) pSrc.at<char>(i, j - 1) != 0;
//				int C = int(~neighbor1 & (neighbor2 | neighbor3))
//						+ int(~neighbor3 & (neighbor4 | neighbor5))
//						+ int(~neighbor5 & (neighbor6 | neighbor7))
//						+ int(~neighbor7 & (neighbor0 | neighbor1));
//				if (C == 1) {
//					/// calculate N
//					int N1 = int(neighbor0 | neighbor1)
//							+ int(neighbor2 | neighbor3)
//							+ int(neighbor4 | neighbor5)
//							+ int(neighbor6 | neighbor7);
//					int N2 = int(neighbor1 | neighbor2)
//							+ int(neighbor3 | neighbor4)
//							+ int(neighbor5 | neighbor6)
//							+ int(neighbor7 | neighbor0);
//					int N = min(N1, N2);
//					if ((N == 2) || (N == 3)) {
//						int E = (neighbor5 | neighbor6 | ~neighbor0)
//								& neighbor7;
//						if (E == 0) {
//							pDst.at<char>(i, j) = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//}
