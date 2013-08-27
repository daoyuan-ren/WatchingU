/*
 * Frame.cpp
 *
 *  Created on: Aug 25, 2013
 *      Author: ren
 */

#include "Frame.h"

Frame::Frame() {
	Grids = 1;
	Statistic = NULL;
}

Frame::Frame(const int& m, const int& n){
	setGrids(m, n);
}

Frame::~Frame() {
	if(Statistic != NULL)
		delete []Statistic;
}

const Mat& Frame::frame(){
	return mFrame;
}

const Info* Frame::info(){
	return Statistic;
}

bool Frame::infoEmpty(){
	return Statistic == NULL ? true : false;
}

void Frame::setGrids(int m, int n){
	Grids = m * n;
	Statistic = new Info[Grids];
}

void Frame::setInfo(int idx, const Info& info){
	if(idx < 0 || idx > Grids)
		return;
	Statistic[idx].setInfo(info);
}

void Frame::show(const char* title){
	if(title != NULL)
		imshow(title, this->mFrame);
	return;
}
