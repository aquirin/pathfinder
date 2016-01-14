// Stat.h: interface for the CStat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STAT_H__8286B477_928F_40C2_AAA2_B98139CA492D__INCLUDED_)
#define AFX_STAT_H__8286B477_928F_40C2_AAA2_B98139CA492D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "base.h"

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <algorithm>
using std::sort;

#include <vector>
using std::vector;

#include <map>
using std::map;

#ifdef linux
	#define _isnan isnan
	#define _finite finite
#endif

#define EPSILON 0.00001

class CStat {
	long cnt;               // number of values
	long err;				// number of errors (overflow values, ex: -1.#IND000000000)
	double sumx;            // sum of values
	double sumx2;           // sum of squares of values
	double vmin, vmax;		// min and max of values
	vector<double> values;		// values for computing the median
	int mem;				// how many values we need to save
	int pos_max;		// position of the max value
	int pos_min;		// position of the min value	

public:
	CStat(void);			// constructor
	CStat(int memory);		// constructor
	void clean();			// empty the object
	void add(double);       // add a value
	void del(double d);		// delete a value
	long np();				// get number of values passed
	long n();               // get number of values counted
	double sum();           // get sum of values
	double gmin();			// get the global min of values (without delete)
	double gmax();			// get the global max of values (without delete)
	double gwidth();	// get the global width of values (without delete)
	double mean();          // get average
	double stdev();         // get standard deviation
	long over();			// get number of overflow values
	double median(void);	// get the median
	double median(double p);	// get the specified median
	static void Test1();
	void print(void);
	double less_frequent(void);	// get the less frequent value
	double most_frequent(void);	// get the most frequent value
	int frequency(double d);	// get the frequency of the value
	void print_distribution(void);	// print the distribution of the values
	int gpos_max();			// print position of the max value
	int gpos_min();			// print position of the min value	
};

#endif // !defined(AFX_STAT_H__8286B477_928F_40C2_AAA2_B98139CA492D__INCLUDED_)
