#include <iostream>
#include <cmath>
using namespace std;

class Triangle;
class Dot {
	friend class Triangle;
	friend istream& operator>>(istream&, Dot&);
private:
	float x;
	float y;
public:
	Dot(float xx = 0.0, float yy = 0.0) : x(xx), y(yy) { };
	float getx() { return x; };
	float gety() { return y; };
};

class Triangle {
private:
	Dot d1;
	Dot d2;
	Dot d3;
public:
	Triangle(Dot& p, Dot& q, Dot& t) : d1(p), d2(q), d3(t) {};
	double radius();
};

double Triangle::radius() {
	float dx12 = d1.x - d2.x;
	float dy12 = d1.y - d2.y;
	float dx13 = d1.x - d3.x;
	float dy13 = d1.y - d3.y;
	float dx23 = d2.x - d3.x;
	float dy23 = d2.y - d3.y;
	double a = sqrt(pow(dx12, 2) + pow(dy12, 2));
	double b = sqrt(pow(dx13, 2) + pow(dy13, 2));
	double c = sqrt(pow(dx23, 2) + pow(dy23, 2));
	double p = (a + b + c) / 2;
	double R = (a*b*c) / (4*sqrt(p*(p - a)*(p - b)*(p - c)));
	return R;
}

istream& operator>>(istream& input, Dot& p) {
	char semicolon = ';';
	input >> p.x >> semicolon >> p.y;
	return input;
}

int main() {
	Dot A, B, C;
	double r;
	cout << "Input xa;ya xb;yb xc;yc or exit" << endl;
	while (cin >> A >> B >> C) {
		Triangle T(A, B, C);
		r = T.radius();
		cout << "R = " << r << endl;
		cout << "\nInput xa;ya xb;yb xc;yc or exit" << endl;
	}
	return 0;
}
