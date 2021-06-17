#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
int main()
{
    const int CHESSBOARD_WIDTH = 7; //input width of  chessboard
    const int CHESSBOARD_HEIGHT = 6; //input height of chessboard
    const float squareSize = 1.5; //input size of a side of a single square in chessboard
    cv::Size corner = cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
    int counter = 13;
    int nimages = 13;
    cv::Size imageSize;
    enum { capturing = 0, calibrated = 1 };
    int mode = capturing;
    char leftfilename[100];
    char rightfilename[100];


    std::vector<cv::Mat> imagePoints1;
    std::vector<cv::Mat> imagePoints2;
    std::vector<std::vector<cv::Point3f>> objectPoints;

    bool found1 = false;
    bool found2 = false;
    int counter2 = 0;
    cv::Mat pointBuf1 = cv::Mat::zeros(54, 2, CV_32FC1);
    cv::Mat pointBuf2 = cv::Mat::zeros(54, 2, CV_32FC1);
    for (int i = 1; i <= counter; i++)
    {
        /*sprintf(leftfilename, "./images/A4_calib/left%d.bmp", i);
        sprintf(rightfilename, "./images/A4_calib/right%d.bmp", i);*/

        sprintf(leftfilename, "./images/sample/stereo_calib/left%d.jpg", i);
        sprintf(rightfilename, "./images/sample/stereo_calib/right%d.jpg", i);


        // const int CHESSBOARD_INTERSECTION_COUNT = CHESSBOARD_WIDTH * CHESSBOARD_HEIGHT;
        cv::Mat imgleft_frame = cv::imread(leftfilename);
        cv::Mat imgright_frame = cv::imread(rightfilename);

        std::cout << "Reading images finished!" << std::endl;

        //cv::Mat imgleft_frame =cv::Mat(480,640,CV_8UC4,s.leftBuffer,4*imgWidth*sizeof(unsigned char));
        //cv::Mat imgright_frame =cv::Mat(480,640,CV_8UC4,s.rightBuffer,4*imgWidth*sizeof(unsigned char));
        imageSize = imgleft_frame.size();

        found1 = findChessboardCorners(imgleft_frame, corner, pointBuf1, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
        found2 = findChessboardCorners(imgright_frame, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT), pointBuf2, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
        std::cout << "found1 : " << found1 << "    found2 : " << found2 << std::endl;
        if (found1)
        {
            cv::Mat gray_image1;
            cvtColor(imgleft_frame, gray_image1, cv::COLOR_BGRA2GRAY);
            cornerSubPix(gray_image1, pointBuf1, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
            drawChessboardCorners(imgleft_frame, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT), pointBuf1, found1);
        }
        if (found2)
        {
            cv::Mat gray_image2;
            cvtColor(imgright_frame, gray_image2, cv::COLOR_BGRA2GRAY);
            cornerSubPix(gray_image2, pointBuf2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
            drawChessboardCorners(imgright_frame, cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT), pointBuf2, found2);
        }
        if (found1 && found2)
        {
            imagePoints1.push_back(pointBuf1);
            imagePoints2.push_back(pointBuf2);
            //sprintf(leftfilename,"newleftcheck%d.jpg",s.counter);
            //sprintf(rightfilename,"newrightcheck%d.jpg",s.counter);
            //cv::imwrite(leftfilename,imgleft_frame);
            //cv::imwrite(rightfilename,imgright_frame);
            counter2 = counter2 + 1;
            std::cout << "counter2 : " << counter2 << std::endl;
        }
        nimages = counter2;
        std::cout << "nimages : " << nimages << std::endl;
        objectPoints.resize(nimages);
        std::cout << "countervalue" << i << std::endl;
    }
    for (int i = 0; i < nimages; i++)
    {
        for (int j = 0; j < CHESSBOARD_HEIGHT; j++)
            for (int k = 0; k < CHESSBOARD_WIDTH; k++)
                objectPoints[i].push_back(cv::Point3f(j * squareSize, k * squareSize, 0));
    }
    std::cout << "check1" << std::endl;
    cv::Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = cv::Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat R, T, E, F;
    std::cout << objectPoints.size() << std::endl;
    std::cout << imagePoints1.size() << std::endl;
    if (imagePoints1.size() == imagePoints2.size())
        std::cout << "samesize" << std::endl;
    if (imagePoints1.size() >= nimages)
    {
        std::cout << "check2" << std::endl;
        double rms = stereoCalibrate(objectPoints, imagePoints1, imagePoints2, cameraMatrix[0], distCoeffs[0],
            cameraMatrix[1], distCoeffs[1], imageSize, R, T, E, F,
            cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_ZERO_TANGENT_DIST +
            cv::CALIB_SAME_FOCAL_LENGTH + cv::CALIB_RATIONAL_MODEL +
            cv::CALIB_FIX_K3 + cv::CALIB_FIX_K4 + cv::CALIB_FIX_K5,
            cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5));

        std::cout << "check3" << std::endl;
        std::cout << "done with RMS error=" << rms << std::endl;
        mode = calibrated;
        std::cout << "calibrated" << std::endl;
    }
    if (mode == calibrated)
    {

        double err = 0;
        int npoints = 0;
        std::vector<cv::Vec3f> lines[2];
        for (int i = 0; i < nimages; i++)
        {
            int npt = (int)imagePoints1[i].rows;
            std::cout << npt << std::endl;
            cv::Mat imgpt1;
            cv::Mat imgpt2;
            //  for(int k = 0; k < 2; k++ )

            imgpt1 = cv::Mat(imagePoints1[i]);
            undistortPoints(imgpt1, imgpt1, cameraMatrix[0], distCoeffs[0], cv::Mat(), cameraMatrix[0]);
            computeCorrespondEpilines(imgpt1, 1, F, lines[0]);


            imgpt2 = cv::Mat(imagePoints2[i]);
            undistortPoints(imgpt2, imgpt2, cameraMatrix[1], distCoeffs[1], cv::Mat(), cameraMatrix[1]);
            computeCorrespondEpilines(imgpt2, 2, F, lines[1]);
            std::cout << "checksdcdufb" << std::endl;
            //std::cout<<"imagepoint"<<imagePoints1[1].at<unsigned int>(1,1)<<std::endl;
            /*  for(int j = 0; j < npt; j++ )
            {
            double errij = fabs(imagePoints1[i].at<double>(j,0) *lines[1][j][0] +imagePoints1[i].at<double>(j,1)*lines[1][j][1] + lines[1][j][2]) +fabs(imagePoints2[i].at<double>(j,0)*lines[0][j][0] +
            imagePoints2[i].at<double>(j,1)*lines[0][j][1] + lines[0][j][2]);
            err += errij;
            }
            npoints += npt;

            }*/
            std::cout << "check8" << std::endl;
            cv::FileStorage fs("./images/sample/stereo_calib/intrinsics.xml", cv::FileStorage::WRITE);
            if (fs.isOpened())
            {
                fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
                    "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
                fs.release();
            }
            else
                std::cout << "Error: can not save the intrinsic parameters\n";

            cv::Mat R1, R2, P1, P2, Q;
            cv::Rect validRoi[2];

            stereoRectify(cameraMatrix[0], distCoeffs[0],
                cameraMatrix[1], distCoeffs[1],
                imageSize, R, T, R1, R2, P1, P2, Q,
                cv::CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

            fs.open("./images/sample/stereo_calib/extrinsics.xml", cv::FileStorage::WRITE);
            if (fs.isOpened())
            {
                fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
                fs.release();
            }
            else
                std::cout << "Error: can not save the intrinsic parameters\n";


        }


        //std::cout << "average reprojection err = " <<  err/npoints <<std::endl;
    }
    return 0;
}