/*
 *This code describes an example of how to segmentate a melanoma image
 *It's used the Otsu method for segmentation and for labelling it's used the distance between melanoma centroid and image center
 */
#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
#include <ctime>
#include <stack>

using namespace cv;
using namespace std;

/*img: Used to store the melanoma image
 *backup: A backup from the melanoma img
 *gray: Grayscale conversion from img
 *binary: gray after segmentation
 *blurred: binary after median 3x3 filter
 *rows,cols: img's size
 *area: variable used to store the coutour's areas, it will used the function 'Moments' to get this value, details can be found here:
 *http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=moments#moments
 *resultBeforeCrop: black image with segmented melanoma but with same original size
 *resultAfterCrop: black image after bounding rectangle crop
 * */

std::stack<clock_t> tictoc_stack;

void tic() {
    tictoc_stack.push(clock());
}

void toc() {
    std::cout << "Time elapsed: "
              << ((double)(clock() - tictoc_stack.top())) / CLOCKS_PER_SEC
              << " sec"<<std::endl;
    tictoc_stack.pop();
}

int main(int argc, char** argv) {
	//detLine();
	//tic();
	Mat gray, binary, blurred,resultBeforeCrop,backup,resultAfterCrop;
	Mat img = imread("images/8.JPG");
	img.copyTo(backup);
	if (!img.data) {
		cout << "File not found" << endl;
		return -1;
	}
	//namedWindow("Original Image", WINDOW_NORMAL);
	//imshow("Original Image", img);
	//waitKey(0);
	int rows = img.rows;
	int cols = img.cols;
	int area = 0;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 0, 255, THRESH_BINARY_INV | CV_THRESH_OTSU);
	medianBlur(binary, blurred, 3);
	//namedWindow("Image after segmentation + median filter", WINDOW_NORMAL);
	//imshow("Image after segmentation + median filter", blurred);
	//waitKey(0);
	/*
	 *Contour detection using findContours function, details can be found here: http://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours#findcontours
	 */
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(blurred, contours, hierarchy, CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
	/*
	 * Creation of a bouding box to enclose the segmented image
	 */
	vector<Rect> boundRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		boundRect[i] = boundingRect(Mat(contours[i]));
	}
	/*
	 * The hierarchy.size() is used to know if some contour is detected, after that a loop through the contour is initiated.
	 * This version excludes areas below 5000 pixels squared, to avoid contours that aren't significant (this need to be way improved)
	 * 'xb' and 'yb' are used to get the contour centroid, more details can be found here: http://docs.opencv.org/trunk/dd/d49/tutorial_py_contour_features.html
	 * 'distance' is the way that its decided what contour is the melanoma, usually the melanoma will be in the center of the image,
	 * so if the image centroid is closer than 900 pixels from the img centroid, then it's considered to be the melanoma contour (Need improvement too)
	 * */
	if (hierarchy.size() > 0) {
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {
			Moments moment = moments((cv::Mat) contours[index]);
			area = moment.m00;
			if (area > 5000) {
				int xb = moment.m10 / area;
				int yb = moment.m01 / area;
				float distance = sqrt(pow((xb - cols / 2), 2) + pow((yb - rows / 2), 2));
				//cout << "area = " << area << "distance = " << distance << endl;
				//circle(img, Point(cols / 2, rows / 2), 50, Scalar(255, 255, 255),5);
				//circle(img, Point(xb, yb), 50, Scalar(0, 255, 0),5);
				//drawContours(img, contours, index, Scalar(0, 255, 0), 5, 8,hierarchy, 0, Point());
				//namedWindow("Detected Points", WINDOW_NORMAL);
				//imshow("Detected Points", img);
				//waitKey(0);
				if (distance < 900) {
					Rect boxContour(boundRect[index].tl(), boundRect[index].br());
					Mat black(img.size(), CV_8UC3, Scalar(0, 0, 0));
					drawContours(black, contours, index, Scalar(255, 255, 255), -1, 8,hierarchy, 0, Point());
					//namedWindow("White filled contour", WINDOW_NORMAL);
					//imshow("White filled contour", black);
					bitwise_and(black,backup,resultBeforeCrop);
					resultAfterCrop = resultBeforeCrop(boxContour);
					vector<Vec3b> color;
					Vec3b bgrPixel = resultAfterCrop.at<Vec3b>(247, 376);
					color.push_back(bgrPixel);
					unsigned char uVal = color[0][0];
					cout << uVal << endl;
					namedWindow("Final Result", WINDOW_NORMAL);
					imshow("Final Result", resultAfterCrop);
					waitKey(0);


				}

			}
		}
	}
	//toc();
	return 0;
}
