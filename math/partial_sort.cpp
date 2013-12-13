/**
 * 	@filename :
 * 		partial_sort.cpp
 * 
 * 	@note :
 * 
 * 
 **/
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <time.h>

using namespace std;

int rand_int()
{
	return rand()%100;
}

void print(vector<int> &v,const char* s)
{
	cout<<s<<endl;
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	cout<<endl;
}

bool cmp(int &a, int &b)
{
	if(a>b)
		return true;
	return false;
}

class compare{
public:
    bool operator()(const int &a,const int &b)
    {
        if(a<b)
            return true;
        return false;
    }
};

int main()
{
	srand(time(NULL));
	vector<int> v;
	generate_n(back_inserter(v),10,rand_int);
	print(v,"产生10个随机数");

	partial_sort(v.begin(),v.begin()+4,v.end());
	print(v,"局部递增排序");

	partial_sort(v.begin(),v.begin()+4,v.end(),cmp);
	print(v,"局部递减排序");

	partial_sort(v.begin(),v.begin()+4,v.end(),compare());
	print(v,"局部递增排序");

	return 0;
}
