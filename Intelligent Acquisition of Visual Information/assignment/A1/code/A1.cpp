#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main() {
    string image_pattern;
    vector<cv::String> image_files;
    image_pattern = "..\\old\\*.bmp";       \\使用时根据图片路径修改该值
    glob(image_pattern, image_files);

    for (int i = 0; i < image_files.size(); i++) {
        Mat src = imread(image_files[i]);

        cout << image_files[i] << ":" << src.cols << "*" << src.rows << endl;
        Scalar gbr = src.at<Vec3b>(600, 800);       \\获取(800,600)的RGB
                                                    \\注意图片的坐标系横纵轴与矩阵的index相反：图片的x轴对应矩阵的y_index

        cout << "The GBR at (800,600) is " << gbr << endl << endl;  
    }

    waitKey(0);

    return 0;
}