#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <vector>


using namespace std;
using namespace cv;
int main() {

    aruco::DetectorParameters detectorParams = aruco::DetectorParameters();
    aruco::Dictionary aruco_dict = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
    aruco::ArucoDetector detector(aruco_dict, detectorParams);

    string url = "https://192.168.0.3:8080/video";
    VideoCapture cap(url);

    Mat image = imread("23.jpg");

    while (true) {
        Mat img;
        cap >> img;


        vector<vector<Point2f>> corners;
        vector<int> ids;
        vector<vector<Point2f>> rejected;
        detector.detectMarkers(img, corners, ids, rejected);

        //aruco::drawDetectedMarkers(img, corners, ids);
        cout<<corners.size();

        if (corners.size() != 0 && corners.size() >= 4)
        {
            Point2f top_left, top_right, bottom_right, bottom_left;
            Size img_size = img.size();

            for (size_t i = 0; i < corners.size(); i++)
            {
                float x = corners[i][0].x;
                float y = corners[i][0].y;
                int width = img.cols;
                int height = img.rows;

                if (x < img_size.width / 2 && y < img_size.height / 2)
                {
                    top_left = Point2f(x, y);
                }
                else if (x > img_size.width / 2 && y < img_size.height / 2)
                {
                    top_right = Point2f(x, y);
                }
                else if (x > img_size.width / 2 && y > img_size.height / 2)
                {
                    bottom_right = Point2f(x, y);
                }
                else if (x < img_size.width / 2 && y > img_size.height / 2)
                {
                    bottom_left = Point2f(x, y);
                }
            }

            if (top_left.x != 0 && top_left.y != 0)
            {

                vector<Point2f> src_points = { top_left, top_right, bottom_right, bottom_left };
                vector<Point2f> dst_points = { Point2f(0, 0), Point2f(img_size.width, 0),Point2f(img_size.width, img_size.height), Point2f(0, img_size.height)};

                Mat resizeImg;
                resize(image, resizeImg, Size(img.cols, img.rows));


                Mat homography = findHomography(dst_points, src_points);
                Mat img_out;

                warpPerspective(resizeImg, img_out, homography, img_size);

                Point polygonPoints[] = { top_left, top_right, bottom_right, bottom_left };
                Scalar_<unsigned char> color(0, 0, 0);

                fillConvexPoly(img, polygonPoints, 4, color);


                img = img_out + img;

                Point pts[] = { top_left, top_right, bottom_right, bottom_left };
                vector<Point> points(pts, pts + sizeof(pts) / sizeof(pts[0]));
                polylines(img, points, true, Scalar(0, 255, 0), 2);

            }
        }

        imshow("Video", img);
        if (waitKey(1) == 'q')
        {
            break;
        }
    }

    return 0;
}
