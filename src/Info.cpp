/*
 * Info.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: ren
 */

#include "Info.h"

Info::Info() {
	maxValue = 0;
	minValue = 255;
	expectation = 0;
	variance = 0;
}

Info::~Info() {
	// TODO Auto-generated destructor stub
}

Info::Info(const Info& info){
	setInfo(info);
}

void Info::setInfo(const Info& info){
	this->maxValue = info.maxValue;
	this->minValue = info.minValue;
	this->expectation = info.expectation;
	this->variance = info.variance;
}

void Info::setMax(int max){
	maxValue = max;
}

void Info::setMin(int min){
	minValue = min;
}

void Info::setExpectation(double exp){
	expectation = exp;
}

void Info::setVariance(double var){
	variance = var;
}

unsigned char Info::max(){
	return maxValue;
}

unsigned char Info::min(){
	return minValue;
}

double Info::expec(){
	return expectation;
}

double Info::var(){
	return variance;
}
