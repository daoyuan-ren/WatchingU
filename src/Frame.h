/*
 * Frame.h
 *
 *  Created on: Aug 25, 2013
 *      Author: ren
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <opencv2/opencv.hpp>
#include "Info.h"
using namespace cv;

class Frame {
	friend class Monitor;
private:
	int Grids;
protected:
	Mat mFrame;
	Info* Statistic;
public:
	Frame();
	Frame(const int&, const int&);
	virtual ~Frame();

	void 	setGrids(int, int);
	void 	setInfo(int, const Info&);
	const	int& grids(){return Grids;}
	const	Mat& frame();
	const	Info* info();
	bool 	infoEmpty();
	void 	show(const char*);
};

#endif /* FRAME_H_ */
