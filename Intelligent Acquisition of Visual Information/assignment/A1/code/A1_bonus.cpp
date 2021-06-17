#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>
using namespace std;
using namespace cv;

int main() {
    string clean_image = "base.bmp";
    string image_pattern;
    vector<cv::String> image_files;
    image_pattern = ".\\pic\\*.bmp";       //使用时根据图片路径修改该值
    glob(image_pattern, image_files);

    Mat clean_src = imread(clean_image);
    

    for (int k = 0; k < image_files.size(); k++) {
        Mat src = imread(image_files[k]);
        cout << image_files[k] << ":";
        double MSE_sum = 0;

        for (int m = 0; m < 3; m++) {      // compute each MSE of RGB

            double temp_sum = 0;

            for (int i = 0; i < src.rows; i++) {
                for (int j = 0; j < src.cols; j++) {
                    Scalar gbr_src = src.at<Vec3b>(i, j);
                    Scalar gbr_clean = clean_src.at<Vec3b>(i, j);

                    temp_sum += (gbr_src[m] - gbr_clean[m]) * (gbr_src[m] - gbr_clean[m]);
                }
            }
            
            MSE_sum += temp_sum / (src.cols * src.rows);
        }

        double MSE_avg = MSE_sum / 3;      //compute average MSE
        double PSNR = 10 * log10(255 * 255 / MSE_avg);    // compute PSNR

        cout << PSNR << endl;

     //   cout << "   MSE_avg: " << MSE_avg << endl;
     //   cout << "   PSNR: " << PSNR << endl;    // the bigger the PSNR, the more similar(banchmark:30)
     //   cout << "   size: " << src.cols << "*" << src.rows << endl;
     //   cout << endl;
    }

    waitKey(0);

    return 0;
}