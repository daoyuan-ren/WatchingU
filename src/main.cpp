/*
 * openImg.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: ren
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Monitor.h"
using namespace std;
using namespace cv;


int main(int argc, char** argv){

	system("ls");

	Monitor m;
//	m.openImg(argc, argv);
	m.videoCap(argc, argv);
//	m.videoRecord("/home/ren/Pictures/record.avi");
	return 0;
}
