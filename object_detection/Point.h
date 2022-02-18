#ifndef POINTD_HEADER
#define POINTD_HEADER
#include <iostream>
#include <vector>
#include <iostream>


using namespace std;

class Pointd {
	string className;
	int left;
	int bottom;
	int right;
	int top;

public:
	Pointd();
	Pointd(string n, int l,int b, int r, int t);
	bool operator ==(Pointd s);
	void setLeft(int l);
	void setRight(int r);
	void setBottom(int b);
	void setTop(int t);
	int getLeft() const;
	int getRight() const;
	int getTop();
	int getBottom();
	string getClass();
	friend ostream& operator<<(ostream& o, Pointd s);
	bool operator <(Pointd s);
	friend bool compare(const Pointd&, const Pointd&);
};
ostream& operator<<(ostream& o, Pointd s);

#endif