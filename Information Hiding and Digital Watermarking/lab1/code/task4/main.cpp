//#include <opencv2/opencv.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/ml.hpp>
//#include <opencv2/ml/ml.inl.hpp>
//#include <stdio.h>
//#include <iostream>
//#include <random>
//#include <algorithm>
//
//using namespace cv;
//using namespace std;
//
//const int M_IS_1 = 1;
//const int M_IS_0 = 0;
//const int NO_WM = -1;
//
//int index = 0;
//
//Mat E_BLIND(Mat Co, Mat Wr, int m, double alpha);
//double D_LC(Mat Cw, Mat Wr);
//Mat generate_watermark(void);
//Mat readin_watermark(string fileName, string variable);
//void compute_result(vector<int> res_real, vector<int> res_exp);
//
//
//int main(int argc, uchar* argv[])
//{
//	float T_lc = 0.7;
//
//	//Mat coverWork = imread(".\\data_bmp\\airplaneGray.bmp", IMREAD_GRAYSCALE);
//	Mat coverWork = imread("..\\data_bmp\\rec.bmp", IMREAD_GRAYSCALE);
//
//	Mat Co;
//	Co.create(coverWork.rows, coverWork.cols, CV_8UC1);
//	coverWork.convertTo(Co, CV_8UC1, 1.0, 0.0);
//
//	string path = "..\\xml_files\\exp2\\temp\\";
//	//string path = ".\\xml_files\\temp\\";
//	vector<String> images;
//
//	vector<float> res_z_lc;
//	vector<int> res_exp;
//	vector<int> res_real;
//
//	glob(path + "*.xml", images);
//	for (int i = 0; i < images.size(); i++) {
//		Mat Wr = readin_watermark(images[i], "Wr");
//
//		if (Wr.empty()) {
//			cout << "image:" << images[i] << " not found!" << endl;
//			exit(-1);
//		}
//		cout << "watermark reference pattern: " << images[i] << endl;
//
//		Mat Cw;
//		for (int m = -1; m < 2; m++) {
//			if (m == -1)
//				Cw = Co;
//			else
//				Cw = E_BLIND(Co, Wr, m, 1);
//
//			float Z_lc = D_LC(Cw, Wr);
//
//			res_z_lc.push_back(Z_lc);
//			res_real.push_back(m);
//			if (Z_lc > T_lc)
//				res_exp.push_back(M_IS_1);
//			else if (Z_lc < -T_lc)
//				res_exp.push_back(M_IS_0);
//			else
//				res_exp.push_back(NO_WM);
//		}
//		cout << endl;
//	}
//
//	cout << "res_z_lc:" << endl;
//	for (int i = 0; i < res_z_lc.size(); i++)
//		cout << res_z_lc[i] << " ";
//	cout << "\n\n";
//
//	cout << "res_real: " << endl;
//	for (int i = 0; i < res_real.size(); i++)
//		cout << res_real[i] << " ";
//	cout << "\n\n res_exp: " << endl;
//	for (int i = 0; i < res_exp.size(); i++)
//		cout << res_exp[i] << " ";
//	cout << "\n\n";
//
//
//	compute_result(res_real, res_exp);
//
//	return 0;
//}
//
//Mat E_BLIND(Mat Co, Mat Wr, int m, double alpha)
//{
//	// judge if the size of Co and Wr is the same
//	//if (Co.rows != Wr.rows || Co.cols != Wr.cols) {
//	//	cout << "ERROR: the size of Co is inconsistent with Wr in E_BLIND!" << endl;
//	//	exit(-1);
//	//}
//
//	Mat Wm = (m == 1 ? Wr : -Wr);
//	Mat Wa = alpha * Wm;
//	Mat Cw;
//
//	Cw.create(Co.rows, Co.cols, CV_8UC1);
//
//	for (int i = 0; i < Co.rows; i++)
//		for (int j = 0; j < Co.cols; j++) {
//			Cw.at<uchar>(i, j) = (uchar)(Wa.at<float>(i, j) + Co.at<uchar>(i, j));
//		}
//
//		//FileStorage fs;
//		//fs.open("D:\\Desktop\\temp\\temp file\\res\\" + to_string(index++) + ".xml", cv::FileStorage::WRITE);
//		//if (fs.isOpened())
//		//{
//		//	fs << "Cw" << Cw;
//		//	fs.release();
//		//}
//		//else
//		//	std::cout << "Error: can not save Cw\n";
//
//
//	return Cw;
//}
//
//double D_LC(Mat Cw, Mat Wr)
//{
//	// judge if the size of Co and Wr is the same
//	//if (Cw.rows != Wr.rows || Cw.cols != Wr.cols) {
//	//	cout << "ERROR: the size of Cw is inconsistent with Wr in D_LC!" << endl;
//	//	exit(-1);
//	//}
//
//	double Z_lc = 0;
//	int N = Cw.rows * Cw.cols;
//	for (int i = 0; i < Cw.rows; i++)
//		for (int j = 0; j < Cw.cols; j++) {
//			//Z_lc += ((float)Cw.at<uchar>(i, j) * Wr.at<float>(i, j));
//			
//		
//		}
//
//	Z_lc /= N;
//	return Z_lc;
//}
//
//
//Mat generate_watermark(void)
//{
//	RNG rnger(getTickCount());
//	int width = 800, height = 800;
//	Mat watermark;
//
//	watermark.create(height, width, CV_32FC1);
//	rnger.fill(watermark, RNG::NORMAL, Scalar::all(0.), Scalar::all(1.));
//
//	return watermark;
//}
//
//void compute_result(vector<int> res_real, vector<int> res_exp)
//{
//	if (res_real.size() != res_exp.size()) {
//		cout << "ERROR: res_real.size != res_exp.size" << endl;
//		exit(-1);
//	}
//
//	int total_num = res_real.size();
//	int accurate_num = 0;
//	int true_pos = 0, true_neg = 0;
//	int false_pos = 0, false_neg = 0;
//
//	for (int i = 0; i < res_real.size(); i++) {
//		if (res_real[i] == res_exp[i])
//			accurate_num++;
//
//		if ((res_real[i] == M_IS_1 || res_real[i] == M_IS_0) && (res_exp[i] == M_IS_1 || res_exp[i] == M_IS_0))
//			true_pos++;
//		else if ((res_real[i] == M_IS_1 || res_real[i] == M_IS_0) && res_exp[i] == NO_WM)
//			false_neg++;
//		else if (res_real[i] == NO_WM && res_exp[i] == NO_WM)
//			true_neg++;
//		else if (res_real[i] == NO_WM && (res_exp[i] == M_IS_1 || res_exp[i] == M_IS_0))
//			false_pos++;
//		else {
//			cout << "ERROR: exception occurs in function compute_result()!" << endl;
//			exit(-1);
//		}
//
//	}
//
//	cout << "false negative rate: " << fixed << setprecision(4) << (float)false_neg / (false_neg + true_pos) << endl;
//	cout << "flase positive rate: " << fixed << setprecision(4) << (float)false_pos / (false_pos + true_neg) << endl;
//	cout << "System accuracy: " << fixed << setprecision(4) << (float) accurate_num / total_num << endl;
//}
//
//Mat readin_watermark(string fileName, string variable)
//{
//	Mat file_matrix;
//	FileStorage opencv_file(fileName, FileStorage::READ);
//
//
//	opencv_file[variable] >> file_matrix;
//	opencv_file.release();
//
//	return file_matrix;
//}