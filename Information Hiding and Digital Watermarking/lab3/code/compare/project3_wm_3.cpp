#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.inl.hpp>
#include <stdio.h>
#include <numeric>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

using namespace cv;
using namespace std;

const double thresh = 0.5;
int state[8][2] = {
	{0, 1},
	{2, 3},
	{4, 5},
	{6, 7},
	{0, 1},
	{2, 3},
	{4, 5},
	{6, 7}
};

Mat E_BLK_8(Mat Co, vector<int>& m, float alpha, int seed);
vector<int> D_BLK_8(Mat Cw, int seed, float Tcc);
Mat E_BLK_8_Trellis(Mat Co, vector<int>& m, float alpha, int seed);
vector<int> D_BLK_8_Trellis(Mat Cw, int seed, float Tcc);
vector<int> generate_message_8(void);
Mat generate_watermark(int rows, int cols, int seed);

int main()
{
	double alpha = sqrt(8);
	int seed = 0;
	double Tcc = 0.01;
	
	vector<vector<int>> m(256, vector<int>(8));

	for (int i = 0; i < 256; i++) {
		int tempi = i;
		int tempj = 1;
		while (tempi > 0) {
			m[i][8 - tempj] = tempi % 2;
			tempi = tempi / 2;
			tempj++;
		}
	}

	vector<double> decode_accuracy;
	vector<double> decode_accuracy_trellis;


	string path = "..\\data_bmp\\";
	vector<String> images;
	glob(path + "*", images);

	vector<int> res_1;
	vector<int> res_2;
	vector<int> res_1_trellis;
	vector<int> res_2_trellis;
	int false_pos_num = 0;
	int false_neg_num = 0;
	int false_pos_num_trellis = 0;
	int false_neg_num_trellis = 0;

	for (int mi = 0; mi < 256; mi++) {
		for (int i = 0; i < images.size(); i++) {
			Mat coverWork = imread(images[i], IMREAD_GRAYSCALE);
			if (coverWork.empty()) {
				cout << "image:" << images[i] << " not found!" << endl;
				exit(-1);
			}
			cout << "coverWork: " << images[i] << endl;

			// 1. without Trellis Code
			Mat Cw = E_BLK_8(coverWork, m[mi], alpha, seed);
			res_1 = D_BLK_8(Cw, seed, Tcc);

			int count_no = 0;
			int accurate_num = 0;

			for (int j = 0; j < 8; j++) {
				if (res_1[j] == -1)
					count_no++;
				if (res_1[j] == m[mi][j])
					accurate_num++;
			}

			double accurate_rate = (double)accurate_num / 8 * 100;
			cout << "The decode accuracy is " << fixed << setprecision(2) << accurate_rate << "%" << endl;
			decode_accuracy.push_back(accurate_rate);

			if (res_1 != m[mi] && count_no >= 4)
				false_neg_num++;

			res_2 = D_BLK_8(coverWork, seed, Tcc);

			count_no = 0;
			for (int j = 0; j < 8; j++) {
				if (res_2[j] == -1)
					count_no++;
			}
			if (count_no < 4)
				false_pos_num++;

			// 2. with Trellis Code
			Cw = E_BLK_8_Trellis(coverWork, m[mi], alpha, seed);
			res_1_trellis = D_BLK_8_Trellis(Cw, seed, Tcc);

			count_no = 0;
			accurate_num = 0;

			for (int j = 0; j < 8; j++) {
				if (res_1_trellis[j] == -1)
					count_no++;
				if (res_1_trellis[j] == m[mi][j])
					accurate_num++;
			}

			accurate_rate = (double)accurate_num / 8 * 100;
			cout << "The decode accuracy is " << fixed << setprecision(2) << accurate_rate << "%" << endl;
			decode_accuracy_trellis.push_back(accurate_rate);

			if (res_1_trellis != m[mi] && count_no >= 4)
				false_neg_num_trellis++;

			res_2_trellis = D_BLK_8_Trellis(coverWork, seed, Tcc);

			count_no = 0;
			for (int j = 0; j < 8; j++) {
				if (res_2_trellis[j] == -1)
					count_no++;
			}
			if (count_no < 4)
				false_pos_num_trellis++;
		}
	}
	cout << "**************** WITHOUT Trellis Code ****************" << endl;
	double sys_decode_accuracy = accumulate(decode_accuracy.begin(), decode_accuracy.end(), 0.0) / (256 * images.size());

	cout << endl << endl;
	cout << "false positive rate = " << fixed << setprecision(2) << (double)false_pos_num / (256 * 2 * images.size()) * 100 << "%" << endl;
	cout << "false negative rate = " << fixed << setprecision(2) << (double)false_neg_num / (256 * 2 * images.size()) * 100 << "%" << endl;
	cout << "The decode accuracy of this system is " << fixed << setprecision(2) << sys_decode_accuracy << "%" << endl;

	cout << "**************** WITH Trellis Code ****************" << endl;
	double sys_decode_accuracy_trellis = accumulate(decode_accuracy_trellis.begin(), decode_accuracy_trellis.end(), 0.0) / (256 * images.size());

	cout << endl << endl;
	cout << "false positive rate = " << fixed << setprecision(2) << (double)false_pos_num_trellis / (256 * 2 * images.size()) * 100 << "%" << endl;
	cout << "false negative rate = " << fixed << setprecision(2) << (double)false_neg_num_trellis / (256 * 2 * images.size()) * 100 << "%" << endl;
	cout << "The decode accuracy of this system is " << fixed << setprecision(2) << sys_decode_accuracy_trellis << "%" << endl;

	getchar();


}


Mat E_BLK_8(Mat Co, vector<int>& m, float alpha, int seed)
{
	Mat Wm(8, 8, CV_32FC1);
	Co.convertTo(Co, CV_32FC1);

	for (int i = 0; i < 8; i++) {
		Mat Wr = generate_watermark(8, 8, seed + i);

		if (i == 0)
			Wm = (m[i] == 1 ? Wr : -Wr);
		else {
			Wm += (m[i] == 1 ? Wr : -Wr);
		}
	}

	// normalization
	Scalar mean, stddev;
	meanStdDev(Wm, mean, stddev);
	double meanVal = mean.val[0];
	double stddevVal = stddev.val[0];

	Wm = Wm - meanVal;
	Wm = Wm / stddevVal;

	// extract mark
	Mat Vo = Mat::zeros(8, 8, CV_32FC1);
	Mat n = Mat::zeros(8, 8, CV_32FC1);
	for(int i = 0; i < Co.rows; i++)
		for(int j = 0; j < Co.cols; j++){
			int id_x = i % 8;
			int id_y = j % 8;
			Vo.at<float>(id_x, id_y) += Co.at<float>(i, j);
			n.at<float>(id_x, id_y)++;
		}

	Vo = Vo/n;

	// mix blind
	Mat Vw(8, 8, CV_32FC1);
	Vw = Vo + alpha * Wm;

	// inv extract mark
	Mat vo_1 = Mat::zeros(8, 8, CV_32FC1);
	Mat n_1 = Mat::zeros(8, 8, CV_32FC1);
	for(int i = 0; i < Co.rows; i++)
		for(int j = 0; j < Co.cols; j++){
			int id_x = i % 8;
			int id_y = j % 8;
			vo_1.at<float>(id_x, id_y) += Co.at<float>(i, j);
			n_1.at<float>(id_x, id_y)++;

		}

	Mat delta(8, 8, CV_32FC1);
	delta = n.mul(Vw) - n_1;

	Mat Cw = Mat::zeros(Co.rows, Co.cols, CV_32FC1);
	for(int i = 0; i < Cw.rows; i++){
		for(int j = 0; j < Cw.cols; j++){
			int id_x = i % 8;
			int id_y = j % 8;
			float oldVal = Co.at<float>(i, j);
			float newVal = Co.at<float>(i, j) + (delta.at<float>(id_x, id_y) / n_1.at<float>(id_x, id_y));
			
			newVal = (newVal > 255 ? 255 : (newVal < 0 ? 0 : newVal));

			Co.at<float>(i, j) = newVal;
			n_1.at<float>(id_x, id_y)--;
			delta.at<float>(id_x, id_y) -= (Co.at<float>(i, j) - oldVal);
		}
	}

	Cw = Co;

	return Cw;
}

vector<int> D_BLK_8(Mat Cw, int seed, float Tcc)
{
	Cw.convertTo(Cw, CV_32FC1);

	// extract mark
	Mat v = Mat::zeros(8, 8, CV_32FC1);
	Mat n = Mat::zeros(8, 8, CV_32FC1);

	for(int i = 0; i < Cw.rows; i++)
		for(int j = 0; j < Cw.cols; j++){
			int id_x = i % 8;
			int id_y = j % 8;
			v.at<float>(id_x, id_y) += Cw.at<float>(i, j);
			n.at<float>(id_x, id_y)++;
		}

	v = v / n;

	// demodulation
	Mat Wr(8, 8, CV_32FC1);
	vector<int> m;

	for (int i = 0; i < 8; i++) {
		Wr = generate_watermark(8, 8, seed + i);
		//write_watermark(Wr, "Wr_D_" + to_string(seed + i));

		double val = 0;
		val = sum(v.mul(Wr))[0];

		double temp = val / (double)(8 * 8);

		//cout << "m[" << i << "]: " << temp << endl;
		if (temp > thresh)
			m.push_back(1);
		else if (temp < -thresh)
			m.push_back(0);
		else
			m.push_back(-1);
	}

	// modulate
	Mat Wm = Mat::zeros(8, 8, CV_32FC1);
	for(int i = 0; i < 8; i++){
		Mat temp_W = generate_watermark(8, 8, seed + i);

		if(m[i] == 1)
			Wm += temp_W;
		else
			Wm -= temp_W;
	}

	// compute correlation coefficiency
	Scalar mean, stddev;
	meanStdDev(v, mean, stddev);
	float meanVal = mean.val[0];
	v = v - meanVal;

	meanStdDev(Wm, mean, stddev);
	meanVal = mean.val[0];
	Wm = Wm - meanVal;

	double vWm = sum(v.mul(Wm))[0];
	double vv = sum(v.mul(v))[0];
	double WmWm = sum(Wm.mul(Wm))[0];
	double cc = 0;

	if(abs(vv * WmWm) < 0.0000001)
		cc = 0;
	else	
		cc = abs(vWm) / sqrt(abs(vv * WmWm));

	if(cc < Tcc){
		m.clear();
		for(int i = 0; i < 8; i++)
			m.push_back(-1);
	}	
	return m;
}

Mat E_BLK_8_Trellis(Mat Co, vector<int>& m, float alpha, int seed)
{
	Mat Wm(8, 8, CV_32FC1);
	Co.convertTo(Co, CV_32FC1);
	int current_state = 0;

	for (int i = 0; i < 10; i++) {
		Mat Wr = generate_watermark(8, 8, seed + current_state * 10 + i - 1);
		if (i < 8 && m[i] == 1) {
			Wm += Wr;
			current_state = state[current_state][1];
		}
		else {
			Wm -= Wr;
			current_state = state[current_state][0];
		}
	}

	// normalization
	Scalar mean, stddev;
	meanStdDev(Wm, mean, stddev);
	double meanVal = mean.val[0];
	double stddevVal = stddev.val[0];

	Wm = Wm - meanVal;
	Wm = Wm / stddevVal;

	// extract mark
	Mat Vo = Mat::zeros(8, 8, CV_32FC1);
	Mat n = Mat::zeros(8, 8, CV_32FC1);
	for (int i = 0; i < Co.rows; i++)
		for (int j = 0; j < Co.cols; j++) {
			int id_x = i % 8;
			int id_y = j % 8;
			Vo.at<float>(id_x, id_y) += Co.at<float>(i, j);
			n.at<float>(id_x, id_y)++;
		}

	Vo = Vo / n;

	// mix blind
	Mat Vw(8, 8, CV_32FC1);
	Vw = Vo + alpha * Wm;

	// inv extract mark
	Mat vo_1 = Mat::zeros(8, 8, CV_32FC1);
	Mat n_1 = Mat::zeros(8, 8, CV_32FC1);
	for (int i = 0; i < Co.rows; i++)
		for (int j = 0; j < Co.cols; j++) {
			int id_x = i % 8;
			int id_y = j % 8;
			vo_1.at<float>(id_x, id_y) += Co.at<float>(i, j);
			n_1.at<float>(id_x, id_y)++;

		}

	Mat delta(8, 8, CV_32FC1);
	delta = n_1.mul(Vw) - vo_1;

	Mat Cw = Mat::zeros(Co.rows, Co.cols, CV_32FC1);
	for (int i = 0; i < Cw.rows; i++) {
		for (int j = 0; j < Cw.cols; j++) {
			int id_x = i % 8;
			int id_y = j % 8;
			float oldVal = Co.at<float>(i, j);
			float newVal = Co.at<float>(i, j) + (delta.at<float>(id_x, id_y) / n_1.at<float>(id_x, id_y));

			newVal = (newVal > 255 ? 255 : (newVal < 0 ? 0 : newVal));

			Co.at<float>(i, j) = newVal;
			n_1.at<float>(id_x, id_y)--;
			delta.at<float>(id_x, id_y) -= (Co.at<float>(i, j) - oldVal);
		}
	}

	Cw = Co;

	return Cw;
}

vector<int> D_BLK_8_Trellis(Mat Cw, int seed, float Tcc)
{
	Mat Wr(8, 8, CV_32FC1);
	Cw.convertTo(Cw, CV_32FC1);

	// extract mark
	Mat v = Mat::zeros(8, 8, CV_32FC1);
	Mat n = Mat::zeros(8, 8, CV_32FC1);

	for (int i = 0; i < Cw.rows; i++)
		for (int j = 0; j < Cw.cols; j++) {
			int id_x = i % 8;
			int id_y = j % 8;
			v.at<float>(id_x, id_y) += Cw.at<float>(i, j);
			n.at<float>(id_x, id_y)++;
		}

	v = v / n;

	// demodulation
	int lc0[8] = { 0 };

	for (int i = 0; i < 8; i++)
		lc0[i]--;
	lc0[1] = 0;

	int m0[8][8] = { 0 };
	int m1[8][8] = { 0 };


	for (int i = 0; i < 10; i++) {
		int lc1[8] = { 0 };
		for (int j = 0; j < 8; j++)
			lc1[j]--;

		for (int current_state = 0; current_state < 8; current_state++) {
			if (lc0[current_state] != -1) {
				Wr = generate_watermark(8, 8, seed + current_state * 10 + i - 1);
				float lc = sum(v.mul(Wr))[0] / (float)(8 * 8);
				int next_state = state[current_state][0];
				if (lc1[next_state] == -1 || lc1[next_state] < lc0[current_state] - lc) {
					lc1[next_state] = lc0[current_state] - lc;
					for (int j = 0; j < 8; j++) {
						m1[next_state][j] = m0[current_state][j];
					}
				}
				if (i < 8) {
					next_state = state[current_state][1];
					if (lc1[next_state] == -1 || lc1[next_state] < lc0[current_state] + lc) {
						lc1[next_state] = lc0[current_state] + lc;
						for (int j = 0; j < 8; j++) {
							m1[next_state][j] = m0[current_state][j];
						}
						m1[next_state][i] = 1;
					}
				}
			}
		}

		for (int j = 0; j < 8; j++) {
			lc0[j] = lc1[j];
			for (int k = 0; k < 8; k++) {
				m0[j][k] = m1[j][k];
			}
		}

	}

	int bestState = 0;
	for (int i = 1; i < 8; i++) {
		if (lc0[i] > lc0[bestState])
			bestState = i;
	}

	vector<int> m_best(8, 0);
	for (int i = 0; i < 8; i++) {
		m_best[i] = m0[bestState][i];
	}

	// modulate
	Mat Wm = Mat::zeros(8, 8, CV_32FC1);
	int current_state = 0;

	for (int i = 0; i < 10; i++) {
		Wr = generate_watermark(8, 8, seed + current_state * 10 + i - 1);
		if (i < 8 && m_best[i] == 1) {
			Wm += Wr;
			current_state = state[current_state][1];
		}
		else {
			Wm -= Wr;
			current_state = state[current_state][0];
		}
	}

	// compute correlation coefficiency
	Scalar mean, stddev;
	meanStdDev(v, mean, stddev);
	float meanVal = mean.val[0];
	v = v - meanVal;

	meanStdDev(Wm, mean, stddev);
	meanVal = mean.val[0];
	Wm = Wm - meanVal;

	double vWm = sum(v.mul(Wm))[0];
	double vv = sum(v.mul(v))[0];
	double WmWm = sum(Wm.mul(Wm))[0];
	double cc = 0;

	if (abs(vv * WmWm) < 0.0000001)
		cc = 0;
	else
		cc = abs(vWm) / sqrt(abs(vv * WmWm));

	if (cc < Tcc) {
		for (int i = 0; i < 8; i++)
			m_best[i] = 2;
	}
	return m_best;
}

vector<int> generate_message_8(void)
{
	vector<int> m;

	for (int i = 0; i < 8; i++) {
		RNG rnger(getTickCount());
		int val = rnger.uniform(0, INT_MAX);
		m.push_back(val % 2);
	}
	return m;
}


Mat generate_watermark(int rows, int cols, int seed)
{
	RNG rnger(seed);
	Mat watermark;

	watermark.create(rows, cols, CV_32FC1);
	rnger.fill(watermark, RNG::NORMAL, Scalar::all(0.), Scalar::all(1.));

	return watermark;
}
