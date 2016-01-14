// Stat.cpp: implementation of the CStat class.
//
//////////////////////////////////////////////////////////////////////

#include "Stat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// constructor
// save an infinite number of values in the table 'values'
CStat::CStat(void)
{
	cnt = 0;
	err = 0;
	sumx = 0.0;
	sumx2 = 0.0;
	vmin = 0.0;
	vmax = 0.0;
	pos_max = 0;
	pos_min = 0;
	mem = -1;
	values.clear();
}


// constructor
// memory = how many values we need to save (0 for no save, -1 for infinite)
// the table 'values' is only used for the functions median/less_frequent/most_frequent/frequency
CStat::CStat(int memory)
{
	cnt = 0;
	err = 0;
	sumx = 0.0;
	sumx2 = 0.0;
	vmin = 0.0;
	vmax = 0.0;
	pos_max = 0;
	pos_min = 0;
	mem = memory;
	values.clear();
}

// empty the object
void CStat::clean()
{
	cnt = 0;
	sumx = 0.0;
	sumx2 = 0.0;
	vmin = 0.0;
	vmax = 0.0;
	pos_max = 0;
	pos_min = 0;
	values.clear();
}

// add a value
void CStat::add(double d)
{
	//if(d<0 && d==0) // Test for "-1.#IND000000000"
	if(_isnan(d) || !_finite(d))	// Test for NaN
	{
		err++;
		return;
	}

	cnt++;
	sumx += d;
	sumx2 += d * d;
	if(cnt==1)
	{
		vmin = d; pos_min = cnt;
		vmax = d; pos_max = cnt;
	}
	else
	{
	
		//printf(" d:%f  min:%f  max:%f  cnt:%d pos_max:%d \n", d, vmin, vmax, cnt, pos_max);
	
		if (d<vmin) { vmin = d; pos_min = (int)cnt;}
		if (d>vmax) { vmax = d; pos_max = (int)cnt;}
	}
	if((values.size()==mem) && (mem>0)) values.erase(values.begin());
	if(mem!=0) values.push_back(d);
}

// delete a value (invalidates the value of min() and max())
void CStat::del(double d)
{
	assert(cnt >= 1);

	cnt--;
	sumx -= d;
	sumx2 -= d * d;
}

// get number of values passed
long CStat::np()
{
	return cnt+err;
}

// get number of values counted (without nAn)
long CStat::n()
{
	return cnt;
}

// get sum of values
double CStat::sum()
{
	//assert(cnt >= 1);
	if (cnt < 1) return 0;
	
	return sumx;
}

// get the global min of values (without delete)
double CStat::gmin()
{
		if (cnt < 1) return 0;
	
	return vmin;
}

// get the global max of values (without delete)
double CStat::gmax()
{
		if (cnt < 1) return 0;
	
	return vmax;
}

// get the global width of values (without delete)
double CStat::gwidth()
{
	return gmax()-gmin();
}

// get average value
double CStat::mean()
{
	//assert(cnt >= 1);
	if (cnt < 1) return 0;
	
	return sumx / double(cnt);
}

// get standard deviation
// L'écart type est calculé à l'aide de la méthode "sans biais", ou "n-1".
double CStat::stdev()
{
	//assert(cnt >= 2);
	if (cnt < 2) return 0;
	
	return sqrt((double(cnt) * sumx2 - sumx * sumx) /
		(double(cnt) * double(cnt - 1)));
}

// get the number of overflow values
long CStat::over(void)
{
	return err;
}

// get the median
double CStat::median(void)
{
	if (cnt < 1) return 0;
	sort(values.begin(),values.end());
	return values[values.size()/2];
}

// get the specified median
double CStat::median(double p)
{
	if (cnt < 1) return 0;
	sort(values.begin(),values.end());
	int n = (int)(p*cnt);
	if(n>=cnt) n=cnt-1;
	return values[n];
}

// Test the class
void CStat::Test1()
{
	CStat st;
	int i;
	
	for (i = 1; i <= 10; i++)
		st.add(double(i));
	
	printf("n = %ld\n", st.n());
	printf("average = %f\n", st.mean());
	printf("standard deviation = %f\n", st.stdev());
	
}

// print somes values
void CStat::print(void)
{
	printf("range=[ %f %f ] mean= %f +- %f, med50=%f, med95=%f\n",
		gmin(), gmax(), mean(), stdev(),
		median(), median(0.95));
}

// get the less frequent value
double CStat::less_frequent(void)
{
	int i;
	double d;
	map<double, int> mapdi;
	std::map<double, int>::iterator p;
	
	for(i=0;i<values.size();i++)
	{
		d = values[i];
		if(mapdi.count(d) == 0) mapdi[d] = 0;
		mapdi[d]++;
	}

	p = mapdi.begin();
	d = (*(mapdi.begin())).first;		// First value
	int n = (*(mapdi.begin())).second;
	while( p != mapdi.end() )
	{
		std::pair<double, int> object = *p;
			if(object.second < n)
			{
				n = object.second;
				d = object.first;
			}
			p++;
	}

	return d;
}

// get the most frequent value
double CStat::most_frequent(void)
{
	int i;
	double d;
	map<double, int> mapdi;
	std::map<double, int>::iterator p; 
	
	for(i=0;i<values.size();i++)
	{
		d = values[i];
		if(mapdi.count(d) == 0) mapdi[d] = 0;
		mapdi[d]++;
	}

	p = mapdi.begin();
	d = (*(mapdi.begin())).first;		// First value
	int n = (*(mapdi.begin())).second;	// Counter
	while( p != mapdi.end() )
	{
		std::pair<double, int> object = *p;
			if(object.second > n)
			{
				n = object.second;
				d = object.first;
			}
			p++;
	}

	return d;
}

// get the frequency of the given double
int CStat::frequency(double d)
{
	int i, n = 0;

	for(i=0;i<values.size();i++)
	{
		if(fabs(d-values[i])<EPSILON) n++;
	}

	return n;
}
/*
typedef std::map< double, int > mapdi_t;
bool lookup_cmp(mapdi_t::iterator lhs, mapdi_t::iterator rhs)
{
	if (lhs->second.value < rhs->second.value)
		return true;
	if (lhs->second.value > rhs->second.value)
		return false;
	return lhs->first < rhs->first;
}
typedef std::set< mapdi_t::iterator, lookup_cmp > lookup_set_t;
*/
// print the distribution of the values
void CStat::print_distribution(void)
{
	int i;
	double d;
	map<double, int> mapdi;
	std::map<double, int>::iterator p; 
	
	for(i=0;i<values.size();i++)
	{
		d = values[i];
		if(mapdi.count(d) == 0) mapdi[d] = 0;
		mapdi[d]++;
	}

	p = mapdi.begin();
	int n = (*(mapdi.begin())).second;
	while( p != mapdi.end() )
	{
		std::pair<double, int> object = *p;
		d = object.first;
		n = object.second;
		p++;
		printf("Value '%f' : %d times.\n", d, n);
	}
}

// get the global min of values (without delete)
int CStat::gpos_min()
{
		if (cnt < 1) return 0;
	
	return pos_min;
}

// get the global max of values (without delete)
int CStat::gpos_max()
{
		if (cnt < 1) return 0;
	
	return pos_max;
}
