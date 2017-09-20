#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	Mat img = imread("images/8.JPG");
	Mat cinza, histograma, binary, borrada, closed2;
	if (!img.data) {
		cout << "Arquivo nao encontrado" << endl;
		return -1;
	}
	int rows = img.rows;
	int cols = img.cols;
	int morph_operator = 1;
	int morph_elem = 0;
	cvtColor(img, cinza, COLOR_BGR2GRAY);
	threshold(cinza, binary, 0, 255, THRESH_BINARY_INV | CV_THRESH_OTSU);
	medianBlur(binary, borrada, 3);

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
	 vector<vector<Point> > contours_poly( contours.size() );
	  vector<Rect> boundRect( contours.size() );
	  vector<Point2f>center( contours.size() );
	  vector<float>radius( contours.size() );
	  for( size_t i = 0; i < contours.size(); i++ )
	  {
	    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
	  }
	float maiorArea = 0;
	float area = 0;
	float perimetro = 0;
	float Mperimetro = 0;
	int numeroLados = 0;
	/*
	 If utilizado para saber se algum contorno foi encontrado
	 */
	if (hierarchy.size() > 0) {
		/*
		 Em caso verdadeiro, um for é realizado para percorrer todos os contornos encontrados
		 */
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {
			/*
			 Recebo o perímetro do contorno e tento fechar possíveis bordas quebradas com 4% do valor do perimetro, valor escolhido empiricamente

			 */
			perimetro = arcLength((cv::Mat) contours[index], true);
			//approxPolyDP(Mat(contours[index]), contornoFechado,
			//		0.04 * perimetro, true);
			//contours[index] = contornoFechado;
			/*
			 Os momentos de cada contorno são salvos na variável 'moment', onde a área do contorno é o moment.m00
			 */
			Moments moment = moments((cv::Mat) contours[index]);

			area = moment.m00;
			if (area > 1000) {
				//cout << area << endl;
				int xb = moment.m10 / area;
				int yb = moment.m01 / area;
				//cout << area <<" "<<xb<<" "<<yb<< endl;
				float distance = sqrt(
						pow((xb - cols / 2), 2) + pow((yb - rows / 2), 2));
				cout << distance << endl;
				img = imread("images/8.JPG");
				circle(img, Point(cols / 2, rows / 2), 50, Scalar(255, 255, 0));
				circle(img, Point(xb, yb), 50, Scalar(0, 255, 0));
				drawContours(img, contours, index, Scalar(0, 255, 0), 2, 8,
						hierarchy, 0, Point());
				namedWindow("ss", WINDOW_NORMAL);
				imshow("ss", img);
				waitKey(0);
				if (distance < 600) {
					img = imread("images/8.JPG");
					rectangle( img, boundRect[index].tl(), boundRect[index].br(), (255, 255, 0), 2, 8, 0 );
					namedWindow("pp", WINDOW_NORMAL);
									imshow("pp", img);
									waitKey(0);
//					circle(img, extLeft, 50, Scalar(0, 255, 0));
				}

			}/*
			 If utilizado para lógica simples de determinação de qual é a maior area de todos os contornos encontrados e contar o numero de lados
			 */
		}
		// Função usada para diferenciar quadrados de retangulos

		/*
		 If utilizado como filtro passa alta, para escrever o arquivo HTML com a distância calculada
		 , não considerar área menor que 500 pixels quadrados e aceitar para cálculo de distância apenas quadrados.
		 */
	}

	//waitKey(0);
	return 0;
}
