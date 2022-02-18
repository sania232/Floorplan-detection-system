#include "Point.h" 
#include <vector>
#include <iostream>

using namespace std;

Pointd::Pointd() {
	className = "";
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;
}
Pointd::Pointd(string n, int l, int r, int t, int b) {
	className = n;
	this->left =l;
	this->right = r;
	top = t;
	bottom = b;
}
void Pointd::setLeft(int l) {
	this->left = l;
}
void Pointd::setRight(int r) {
	this->right = r;
}
void Pointd::setTop(int t) {
	this->top = t;
}
void Pointd::setBottom(int b) {
	this->bottom = b;
}
int Pointd::getLeft() const {
	return left;
}
int Pointd::getRight() const{
	return right;
}
int Pointd::getTop() {
	return top;
}
int Pointd::getBottom() {
	return bottom;
}
string Pointd::getClass() {
	return className;
}
bool Pointd::operator ==(Pointd s) {
	if (this->className == s.className) {
		return 1;
	}
	return 0;
}
ostream& operator<<(ostream& o, Pointd s) {
	o << s.className << " (" << s.left <<","<<s.top<<") "<<
		" (" << s.left << "," << s.bottom << ") " << 
		" (" << s.right << "," << s.top << ") " <<
		" (" << s.right << "," << s.bottom << ") " << endl;
	return o;
}
bool Pointd::operator <(Pointd s) {
	if (this->bottom < s.bottom) {
		return 1;
	}
	else {
		return 0;
	}
}
bool compare(const Pointd& a, const Pointd& b)
{
	return a.right < b.right;
}
