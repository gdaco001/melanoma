#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	Mat img = imread("images/7.JPG");
	Mat cinza, histograma, binary, borrada, closed2,result,cortada;
	if (!img.data) {
		cout << "Arquivo nao encontrado" << endl;
		return -1;
	}
	int rows = img.rows;
	int cols = img.cols;
	cvtColor(img, cinza, COLOR_BGR2GRAY);
	threshold(cinza, binary, 0, 255, THRESH_BINARY_INV | CV_THRESH_OTSU);
	medianBlur(binary, borrada, 3);
	Mat teste;
		namedWindow("teste inicial", WINDOW_NORMAL);
		imshow("teste inicial", borrada);
		waitKey(0);
	/*
	 Detecção de contornos usando a função findContours
	 */
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(borrada, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE);
	vector<Point> contornoFechado; // usado para fechamento de bordas
	/*
	 Variáveis 'maiorArea' e 'área' usadas para determinar o maior valor de área e a área de cada objeto
	 */
	vector<Rect> boundRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		boundRect[i] = boundingRect(Mat(contours[i]));
	}
	float area = 0;
	if (hierarchy.size() > 0) {
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {

			//perimetro = arcLength((cv::Mat) contours[index], true);
			Moments moment = moments((cv::Mat) contours[index]);
			area = moment.m00;
			if (area > 5000) {

				int xb = moment.m10 / area;
				int yb = moment.m01 / area;
				//cout << area <<" "<<xb<<" "<<yb<< endl;
				float distance = sqrt(
						pow((xb - cols / 2), 2) + pow((yb - rows / 2), 2));
				cout << "area = " << area << "distancia = " << distance << endl;
				img = imread("images/7.JPG");
				circle(img, Point(cols / 2, rows / 2), 50, Scalar(255, 255, 0));
				circle(img, Point(xb, yb), 50, Scalar(0, 255, 0));
				drawContours(img, contours, index, Scalar(0, 255, 0), 2, 8,
						hierarchy, 0, Point());
				namedWindow("contorno", WINDOW_NORMAL);
				imshow("contorno", img);
				waitKey(0);
				if (distance < 900) {
					img = imread("images/7.JPG");
					Rect a(boundRect[index].tl(), boundRect[index].br());

					Mat preta(img.size(), CV_8UC3, Scalar(0, 0, 0));
					drawContours(preta, contours, index, Scalar(255, 255, 255), -1, 8,
							hierarchy, 0, Point());

					namedWindow("pp", WINDOW_NORMAL);
					imshow("pp", img);
					bitwise_and(preta,img,result);
					teste = result(a);
					namedWindow("gg", WINDOW_NORMAL);
					imshow("gg", teste);
					waitKey(0);
				}

			}
		}
	}

	//waitKey(0);
	return 0;
}
