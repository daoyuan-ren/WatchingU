/*
 * Monitor.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: ren
 */

#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "Monitor.h"
#include "Info.h"
using namespace std;

Monitor::Monitor() {
	grid_m 		= 5;
	grid_n 		= 5;
	frame_rate 	= 20;

	//	video source 0 is from configuration
	cap.open(0);
	if(!cap.isOpened()){
		cerr << "failed to open camera 0" << endl;
	}
}

Monitor::~Monitor() {
}

void Monitor::openImg(int fileNum, char** file){
//	Mat head, mask_pre, mask_post;
//	Mat sobel, mobel, mobel1, sub_mm, sub_mh;

	frame_pre.mFrame = imread(file[1], 1);
	frame.mFrame = imread(file[2], 1);
	frame_post.mFrame = imread(file[3], 1);


	if(fileNum !=4 || !frame_pre.frame().data || !frame_post.frame().data || !frame.frame().data){
		cout << "no image data." << endl;
		return;
	}
	cout << "(" << 1 << ", " << file[1] << ", " << frame_pre.frame().rows << ", " << frame_pre.frame().cols  << ")" << endl;
	cout << "(" << 2 << ", " << file[2] << ", " << frame.frame().rows << ", " << frame.frame().cols  << ")" << endl;
	cout << "(" << 3 << ", " << file[3] << ", " << frame_post.frame().rows << ", " << frame_post.frame().cols  << ")" << endl;

	cvtColor(frame_pre.mFrame, frame_pre.mFrame, CV_BGR2GRAY);
	cvtColor(frame_post.mFrame, frame_post.mFrame, CV_BGR2GRAY);
	cvtColor(frame.mFrame, frame.mFrame, CV_BGR2GRAY);

//	getSobel(mask_pre, sobel);
//	getSobel(mask_post, mobel1);
//	getSobel(head, mobel);
//	subtract(sobel, mobel, sub);
	subFrame(frame_pre, frame, sub_mh);
	subFrame(frame_pre, frame_post, sub_mm);

	if(detectDiff(sub_mh, "sub_mh", 5.0, 100, grid_m, grid_n))
		cout << "<- true ->" << endl;
	else
		cerr << "<- false ->" << endl;
	if(detectDiff(sub_mm, "sub_mm", 5.0, 100, grid_m, grid_n))
		cout << "<- true ->" << endl;
	else
		cerr << "<- false ->" << endl;
//	getStatistic(frame_pre,"mask", 4, 4);
//	getStatistic(frame_post, "head", 4, 4);
//	getStatistic(sub_mh, "pre-head", 4, 4);
//	getStatistic(sub_mm, "pre-post", 4, 4);
}

int Monitor::getPixel(const Frame& img, int x, int y){
	Scalar intensity = img.mFrame.at<uchar>(Point(x, y));

	int blue = intensity.val[0];
//	int green = intensity.val[1];
//	int red = intensity.val[2];
//	cout << "[" << blue << ", " << green << ", " << red << "]" << endl;
	return blue;
}

void Monitor::videoCap(const char* const file, int interval){
	Mat current, back, fore, sub;
	vector<vector<Point> > contours;

	unsigned int cnt = 0;
	struct timeval start, pause, end;
	long mtime, seconds, useconds;

	subtractor.set("nmixtures", 3);
	subtractor.set("detectShadows", false);

#ifdef DEBUG
	cap >> current;
	writer.open("/home/ren/Pictures/record.avi", CV_FOURCC('D', 'I', 'V', 'X'), frame_rate, current.size());
#endif

	cap >> frame.mFrame;
	writer.open(file, CV_FOURCC('D', 'I', 'V', 'X'), frame_rate, frame.mFrame.size());
	if(!writer.isOpened()){
				cerr << "failed to open video record." << endl;
	}
	for(;;){
		gettimeofday(&start, NULL);

		cap >> frame.mFrame;
		frame.show("Frame");
		if(++cnt % interval == 0){
//			cnt = 0;
			subtractor.operator ()(frame.mFrame, frame_pre.mFrame);
			erode(frame_pre.mFrame, frame_pre.mFrame, Mat());
			dilate(frame_pre.mFrame, frame_pre.mFrame, Mat());
//			writer << frame.mFrame;
			if(detectDiff(frame_pre, "frame_pre", 2.0, 5, grid_m, grid_n)){
				gettimeofday(&pause, NULL);
				seconds = pause.tv_sec - start.tv_sec;
				cout << "<------------" << cnt << "----true-------------------->" << endl;
			}else{
				// TODO deal with the situation that no motion detected
			}
#ifdef DEBUG
		cap >> current;
		frame.mFrame = current;
//		cvtColor(frame.mFrame, frame.mFrame, CV_BGR2GRAY);
		subtractor.operator()(current, fore);
//		subtractor.getBackgroundImage(back);
		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
//		findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//		drawContours(current, contours, -1, Scalar(0, 0, 255), 2);
		frame_pre.mFrame = fore;

		if(switchON == true){
			writer << frame.mFrame;
		}
//		else if(writer.isOpened() && pau_ed - pau_bg > 60) {
//
//		}
		if(detectDiff(frame_pre, "frame_pre", 2.0, 5, grid_m, grid_n) && cnt > 10){
			switchON = true;
			pau_ed = pau_ed < pau_bg ? cnt : pau_ed;
//			cout << "<-----------------------------"<< cnt << "------ true ------------------------------------------>" << endl;
		}else{
			switchON = false;
			pau_bg = cnt;
		}

//		frame_pre.show("Fore");
		frame.show("Frame");
//		imshow("Background", back);
//		subtract(frame, back, sub);
//		imshow("Subtraction", sub);

		if(waitKey(30) >= 0)
			break;
#endif
		}
		writer << frame.mFrame;
		if(waitKey(30) > 0)
			break;
		gettimeofday(&end, NULL);
		seconds = end.tv_sec - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ((seconds)*1000 + useconds/1000) + 0.5;
//		cout << "time: " << mtime << "ms" << endl;
	}
}

void Monitor::videoRecord(const char* file){
	struct timeval start, end;
	long mtime, seconds, useconds;

	cap >> frame.mFrame;
	writer.open(file, CV_FOURCC('D', 'I', 'V', 'X'), frame_rate, frame.mFrame.size());
	if(!writer.isOpened()){
		cerr << "failed to open video record." << endl;
	}
	for(;;){
		gettimeofday(&start, NULL);
		cap >> frame.mFrame;
		frame.show("Frame");
		writer << frame.mFrame;
		if(waitKey(30) >= 0)
			break;
		gettimeofday(&end, NULL);
		seconds = end.tv_sec - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ((seconds)*1000 + useconds/1000) + 0.5;
		cout << "time: " << mtime << "ms" << endl;
	}
}

void Monitor::getSobel(const Frame& src, Frame& dst){
	Mat sobelX, sobelY;
	GaussianBlur(src.mFrame, src.mFrame, Size(9, 9), 1.5, 1.5);
	Sobel(src.mFrame, sobelX, -1, 0, 1);
	Sobel(src.mFrame, sobelY, -1, 1, 0);
	dst.mFrame = sobelX + sobelY;
}

void Monitor::getStatistic(Frame& frame, const char* const fileName, int m, int n){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
//			cout << "[" << i*(frame.rows/m) << "," << j*(frame.cols/n) << "]";
			frame.Statistic[i*m+j] = statistics(frame.mFrame, fileName, i*(frame.frame().rows/m), j*(frame.frame().cols/n), frame.frame().rows/m, frame.frame().cols/n);
//			cout << " ";
		}
//		cout << endl;
	}
	grid(frame.mFrame, m, n);

	return;
}

Info Monitor::statistics(const Mat& frame, const char* const filename, int x, int y, int rows, int cols){
	int min, max;
	unsigned int sum = 0;
	double var = 0;
	double exp = 0;
	Info info;

	for(int i = x; i < x+rows; i++){
		sum = 0;
		min = frame.at<uchar>(i,0);
		max = frame.at<uchar>(i,0);
		sum += frame.at<uchar>(i,0);
		for(int j = y; j < y+cols; j++){
			min = min > frame.at<uchar>(i, j) ? frame.at<uchar>(i, j) : min;
			max = max < frame.at<uchar>(i, j) ? frame.at<uchar>(i, j) : max;
			sum += frame.at<uchar>(i,j);
		}
		exp += sum / cols;
	}
	exp /= rows;
	for(int i = x; i < x+rows; i++){
		for(int j = 0; j < y+cols; j++){
			var = (frame.at<uchar>(i,j) - exp) * (frame.at<uchar>(i,j) - exp);
		}
	}
	info.setMin(min);
	info.setMax(max);
	info.setExpectation(exp);
	info.setVariance(var);
//	cout << "(" << exp << ", " << var <<  ")";
	return info;
}

void Monitor::grid(Mat& frame, const int& m, const int& n){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < frame.cols; j++){
			frame.at<uchar>(i*(frame.rows/m), j) = 255;
		}
	}
	for(int i = 1; i < n; i++){
			for(int j = 0; j < frame.rows; j++){
				frame.at<uchar>(j, i*(frame.cols/n)) = 255;
			}
	}
}

bool Monitor::detectDiff(Frame& frame, const char* fileName, const double& exp, const double& var, const int& m , const int& n){

	bool 	diff = false;
	int		diff_cnt = 0;
	double percent = 0;

#ifdef DEBUG
	if(fileName!=NULL)
		cout << fileName << endl;
#endif

	frame.setGrids(m, n);
	getStatistic(frame, fileName, m, n);

	for(int i = 0; i < frame.grids(); i++){
		if(frame.Statistic[i].expec()> exp || frame.Statistic[i].var() > var){
			diff_cnt++;
#ifdef DEBUG
			cout << "[" << i << ", " << frame.Statistic[i].expec() << ", " << frame.Statistic[i].var() << "]" << endl;
#endif
		}
	}
	percent = (double)diff_cnt / (m*n);
//	cout << "<" << diff_cnt << "/" << m*n << ", " << percent << ">" << endl;

//	0.1 from configuration
	if(percent > 0.1)
		diff = true;

#ifdef DEBUG
	imshow(fileName, frame.mFrame);
	waitKey(0);
#endif
	return diff;
}

void Monitor::subFrame(const Frame& src, const Frame& dst, Frame& rst){
	subtract(src.mFrame, dst.mFrame, rst.mFrame);
}
