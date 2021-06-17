#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    string image_pattern_left, image_pattern_right;
    vector<cv::String> image_files;
    image_pattern_left = "./images/A4/dragon/left3.bmp";       //使用时根据图片路径修改该值
    image_pattern_right = "./images/A4/dragon/right3.bmp";

    //glob(image_pattern, image_files);

    //for (int i = 0; i < image_files.size(); i++) {
    //    Mat src = imread(image_files[i]);

    //    cout << image_files[i] << ":" << src.cols << "*" << src.rows << endl;
    //    Scalar bgr = src.at<Vec3b>(600, 800);       //获取(800, 600)的RGB
    //        //注意图片的坐标系横纵轴与矩阵的index相反：图片的x轴对应矩阵的y_index

    //        cout << "The bgr at (800,600) is " << bgr << endl << endl;
    //}

    Mat left_mod_mat = (Mat_<double>(3, 3) << 1.003228199, -0.3735089407, 0.1083727366, -8.042056887e-3, 1.095595277, -0.4413554282, 0, 0, 255 / 253);
    Mat right_mod_mat = (Mat_<double>(3, 3) << 1.003555441, -0.4250331329, 0.09409637228, -8.974101952e-3, 1.103227782, -0.4508612249, 0, 0, 1.010173037);

    Mat image_frame_left = imread(image_pattern_left);
    Mat image_frame_right = imread(image_pattern_right);
    cout << image_pattern_left << ": " << image_frame_left.cols << "*" << image_frame_left.rows << endl;
    cout << image_pattern_right << ": " << image_frame_right.cols << "*" << image_frame_right.rows << endl;

    if (image_frame_left.rows != image_frame_right.rows || image_frame_left.cols != image_frame_right.cols) {
        cout << "ERROR: The images taken from left and right are not compatible!" << endl;
        exit(1);
    }

    // Do the RGB modification
    for (int i = 0; i < image_frame_left.rows; i++) {
        for (int j = 0; j < image_frame_left.cols; j++) {
            Vec3b& left_pixel_bgr = image_frame_left.at<Vec3b>(i, j);
            Vec3b& right_pixel_bgr = image_frame_right.at<Vec3b>(i, j);
            Mat left_temp;
            Mat right_temp;

            Mat left_pixel_rgb = (Mat_<double>(1, 3) << left_pixel_bgr.val[2], left_pixel_bgr.val[1], left_pixel_bgr.val[0]);
            Mat right_pixel_rgb = (Mat_<double>(1, 3) << right_pixel_bgr.val[2], right_pixel_bgr.val[1], right_pixel_bgr.val[0]);

            left_temp = left_pixel_rgb * left_mod_mat;
            right_temp = right_pixel_rgb * right_mod_mat;

            left_pixel_bgr.val[0] = (int)left_temp.at<double>(0, 2);
            left_pixel_bgr.val[1] = (int)left_temp.at<double>(0, 1);
            left_pixel_bgr.val[2] = (int)left_temp.at<double>(0, 0);

            right_pixel_bgr.val[0] = (int)right_temp.at<double>(0, 2);
            right_pixel_bgr.val[1] = (int)right_temp.at<double>(0, 1);
            right_pixel_bgr.val[2] = (int)right_temp.at<double>(0, 0);

        }
    }

    imwrite("./images/A4/dragon/left3_mod.bmp", image_frame_left);
    imwrite("./images/A4/dragon/right3_mod.bmp", image_frame_right);


    waitKey(0);

    return 0;
}