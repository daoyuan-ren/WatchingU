/*
 * Info.h
 *
 *  Created on: Aug 27, 2013
 *      Author: ren
 */

#ifndef INFO_H_
#define INFO_H_

class Info {
public:
	Info();
	Info(const Info&);
	virtual ~Info();

	void setInfo(const Info&);
	void setMax(int);
	void setMin(int);
	void setExpectation(double);
	void setVariance(double);

	unsigned char max();
	unsigned char min();
	double expec();
	double var();
private:
	unsigned char maxValue;
	unsigned char minValue;

	double expectation;
	double variance;
};

#endif /* INFO_H_ */
