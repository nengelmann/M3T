// SPDX-License-Identifier: MIT
// Copyright (c) 2023 Nico Engelmann

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat src = Mat::zeros(Size(10,10),CV_8UC1);
    Ptr<Feature2D> surf = xfeatures2d::SURF::create();
    vector<KeyPoint> keypoints;
    Mat descriptors;
    surf->detectAndCompute(src, Mat(), keypoints, descriptors);
    return 0;
}