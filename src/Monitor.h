/*
 * Monitor.h
 *
 *  Created on: Aug 23, 2013
 *      Author: ren
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <opencv2/opencv.hpp>
#include "Frame.h"
#include "Info.h"
using namespace cv;

class Monitor {
//	friend class Frame;
private:
	unsigned int grid_m;
	unsigned int grid_n;
	unsigned int frame_rate;
	Frame frame_pre, frame, frame_post;
	Frame sub_mh, sub_mm;
	VideoCapture cap;
	BackgroundSubtractorMOG2 subtractor;
	VideoWriter writer;
public:
	Monitor();
	virtual ~Monitor();

	void 	openImg(int, char**);
	void 	videoCap(int, char**);
	void	videoRecord(const char*);
	int 	getPixel(const Frame&, int, int);
	void	getSobel(const Frame&, Frame&);
	void	getStatistic(Frame&, const char* const, int, int);
	bool	detectDiff(Frame&, const char*, const double&, const double&, const int&, const int&);
private:
	Info 	statistics(const Mat&, const char* const, int, int, int, int);
	void	grid(Mat&, const int&, const int&);
	void	subFrame(const Frame& src, const Frame& dst, Frame& rst);

};

#endif /* MONITOR_H_ */
